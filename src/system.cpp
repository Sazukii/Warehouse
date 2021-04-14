//Include the librarys
#include "GrabArm.h"
#include "LinkedList.h"
#include "Conveyor.h"
#include "Queue.h"
#include "Mqtt.h"
#include "credentials.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <unistd.h>
#include <pigpio.h>

int blockPriority = 0;

//PiXtend V2 -L- input data
struct pixtInV2L *InputData;

//PiXtend V2 -L- output data
struct pixtOutV2L *OutputData;

//PiXtend V2 -L- DAC Output Data
struct pixtOutDAC *OutputDataDAC;

//Create nullptr's to instantiate in Setup
Queue *queue = nullptr;
LinkedList *storage = nullptr;
GrabArm *grabArm = nullptr;
ICommunication *mqtt = nullptr;
Conveyor *conveyor = nullptr;

//Create SystemStates
SystemStates currentSystemState = CALIBRATING;

//Create JSON Storage
nlohmann::json jsonStorage;
nlohmann::json block1 = nlohmann::json::object();
nlohmann::json block2 = nlohmann::json::object();
nlohmann::json block3 = nlohmann::json::object();
nlohmann::json block4 = nlohmann::json::object();
nlohmann::json block5 = nlohmann::json::object();
nlohmann::json block6 = nlohmann::json::object();
nlohmann::json block7 = nlohmann::json::object();
nlohmann::json block8 = nlohmann::json::object();
nlohmann::json block9 = nlohmann::json::object();

//Create JSON States
nlohmann::json jsonState;
nlohmann::json stateArm = nlohmann::json::object();
nlohmann::json stateConveyor = nlohmann::json::object();

void Setup()
{
    //Instantiate PiXtend structs
    InputData = new pixtInV2L;
    OutputData = new pixtOutV2L;
    OutputDataDAC = new pixtOutDAC;

    //Instantiate Queue
    queue = new Queue(20);

    //Instantiate storage
    storage = new LinkedList();
    storage->MakeList(3, 3);

    //Instantiate MQTT
    mqtt = new Mqtt();
    mqtt->Setup();
    //Configure SPI
    Spi_SetupV2(0);
    Spi_SetupV2(1);
    OutputData->byDigitalOut0 |= 0b11;

    //Write Data in Structure OutputData
    OutputData->byModelOut = 76; // Set model as handshake, PiXtend V2 -L- = 76
    OutputData->byGPIOCtrl = 32; //GPIO1 is used with DHT22 sensor
    OutputDataDAC->wAOut0 = 1023;
    OutputDataDAC->wAOut1 = 1023;
    gpioInitialise();
    //Instantiate Motors, Encoders and GrabArm

    IMotor *horizontalMotor = new Motor(BDO1_Motor_H_Positive, BDO1_Motor_H_Negative, &OutputData->byDigitalOut1);
    IEncoder *horizontalEncoder = new Encoder(19, 26);
    IMotor *verticalMotor = new Motor(BDO1_Motor_V_Up, BDO1_Motor_V_Down, &OutputData->byDigitalOut1);
    IEncoder *verticalEncoder = new Encoder(20, 21);
    IMotor *armMotor = new Motor(BDO0_Arm_FWD, BDO0_Arm_BWD, &OutputData->byDigitalOut0);
    IMotor *conveyorMotor = new Motor(BDO0_Conveyor_FWD, BDO0_Conveyor_BWD, &OutputData->byDigitalOut0);

    conveyor = new Conveyor(conveyorMotor, InputData, OutputData);
    conveyor->StopConveyor();
    grabArm = new GrabArm(InputData, OutputData, horizontalMotor, horizontalEncoder, verticalMotor, verticalEncoder, armMotor);

    printf(" --- Setup complete! --- \n");
}

void UpdateStorage()
{
    block1["color"] = storage->SearchByIndex(0, 0);
    block2["color"] = storage->SearchByIndex(1, 0);
    block3["color"] = storage->SearchByIndex(2, 0);
    block4["color"] = storage->SearchByIndex(0, 1);
    block5["color"] = storage->SearchByIndex(1, 1);
    block6["color"] = storage->SearchByIndex(2, 1);
    block7["color"] = storage->SearchByIndex(0, 2);
    block8["color"] = storage->SearchByIndex(1, 2);
    block9["color"] = storage->SearchByIndex(2, 2);

    jsonStorage["stock"] = {block1, block2, block3, block4, block5, block6, block7, block8, block9};
    std::string jsonDump = jsonStorage.dump();
    char *msg = &jsonDump[0];
    mqtt->Send(TOPIC_STORAGE, msg);
}

void UpdateState(std::string armState, std::string conveyorState)
{
    stateArm["name"] = "Arm";
    stateArm["state"] = armState;
    stateConveyor["name"] = "Conveyor";
    stateConveyor["state"] = conveyorState;

    jsonState["states"] = {stateArm, stateConveyor};
    std::string jsonDump = jsonState.dump();
    char *msg = &jsonDump[0];
    mqtt->Send(TOPIC_STATES, msg);
}

void HandleCalibrateState()
{
    UpdateState("Calibrating", "Idle");
    grabArm->Calibrate();
    printf(" --- Calibrated --- \n");
    currentSystemState = IDLE;
}
void HandleIdleState(Block *blockColor, std::string *command, bool *idle_flag)
{
    if (*idle_flag == true)
    {
        UpdateState("Idle", "Idle");
        *idle_flag = false;
    }
    char *msg = mqtt->Receive();


    if (msg != nullptr)
    {
        printf("1: %s \n", msg);
        std::string s = msg;
        std::string delimiter = ":";
        size_t pos = 0;

        pos = s.find(delimiter);
        *command = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        std::string color = s;
        std::cout << "Command: " << *command << std::endl;
        std::cout << "Block: " << color << std::endl;
        if (color == "Red")
        {
            *blockColor = red;
        }
        else if (color == "White")
        {
            *blockColor = white;
        }
        else if (color == "Blue")
        {
            *blockColor = blue;
        }
        else
        {
            *blockColor = empty;
        }

        if (*blockColor != empty)
        {
            if (*command == "requestFromCrane" || *command == "freeSpaceAvailable")
            {
                currentSystemState = SEARCHING_CONTAINER;
            }
        }
    }
}
void HandleSearchingState(Block *blockColor, std::string *command, Container **container)
{
    if (*command == "requestFromCrane")
    {
        UpdateState("Searching for block", "Idle");

        *container = storage->SearchContainer(*blockColor);
        if (*container == nullptr)
        {
            printf(" --- No blocks of this color --- \n");
            char msg[] = "noBlockOfColor";
            mqtt->Send(TOPIC_CRANE, msg);
            currentSystemState = IDLE;
        }
        else
        {
            printf(" --- Found Red Block --- \n");
            currentSystemState = WAITING_FOR_MESSAGE;
        }
    }
    else if (*command == "freeSpaceAvailable")
    {
        UpdateState("Searching for free space", "Idle");
        *container = storage->SearchContainer(empty);
        if (*container == nullptr)
        {
            char msg[] = "noSpaceForBlock";
            mqtt->Send(TOPIC_CRANE, msg);
            currentSystemState = IDLE;
        }
        else
        {
            printf(" --- Found empty container --- \n");
            currentSystemState = WAITING_FOR_MESSAGE;
        }
    }
    else
    {
        currentSystemState = IDLE;
    }
}
void GrabbingContainer(Container *container)
{
    UpdateState("Getting container", "Idle");
    int posx, posy;
    switch (container->GetXPosition())
    {
    case 0:
        posx = xleft;
        break;
    case 1:
        posx = xmiddle;
        break;
    case 2:
        posx = xright;
        break;
    }
    switch (container->GetYPosition())
    {
    case 0:
        posy = ytop;
        break;
    case 1:
        posy = ymiddle;
        break;
    case 2:
        posy = ybottom;
        break;
    }
    printf(" --- Grabbing Container --- \n");
    grabArm->GrabContainer(posx, posy);
    printf(" --- Grabbed Container --- \n");
    UpdateState("Dropping container", "Idle");
    printf(" --- Dropping Container at Conveyor --- \n");
    grabArm->DropContainer(xconveyor, yconveyor);
    printf(" --- Dropped Container --- \n");
    UpdateState("Idle", "Moving to exit");

    conveyor->MoveToExit();
    sleep(1);
    conveyor->StopConveyor();
}
void HandleWaitingForMessageState(Block *blockColor, std::string *command, Container **container, bool *waiting_for_message_flag)
{
    if (*waiting_for_message_flag == true)
    {
        if (*command == "requestFromCrane")
        {
            char msg[] = "blockReady";
            mqtt->Send(TOPIC_CRANE, msg);
        }
        else if (*command == "freeSpaceAvailable")
        {
            char msg[] = "containerReady";
            mqtt->Send(TOPIC_CRANE, msg);
        }

        UpdateState("Idle", "Idle");
        *waiting_for_message_flag = false;
    }

    char *msg = mqtt->Receive();

    if (msg != nullptr)
    {
        printf("2: %s \n", msg);
        std::string message = msg;
        if (message == "containerFilled")
        {
            (*container)->SetBlock(*blockColor);
            blockPriority += 1;
            (*container)->SetPriority(blockPriority);
            *waiting_for_message_flag = true;
            UpdateStorage();
        }
        else if (message == "blockWasTaken")
        {
            (*container)->SetBlock(empty);
            *waiting_for_message_flag = true;
            UpdateStorage();
        }
    }
}
void DroppingContainer(Container *container, uint8_t *count)
{
    UpdateState("Idle", "Moving to storage");
    conveyor->MoveToWarehouse();
    sleep(1);
    conveyor->StopConveyor();
    printf(" --- Grabbing Container --- \n");
    UpdateState("Grabbing Container", "Idle");
    grabArm->GrabContainer(xconveyor, yconveyor);
    printf(" --- Grabbed Container --- \n");
    printf(" --- Dropping Container in storage --- \n");
    UpdateState("Dropping Container", "Idle");
    int posx, posy;
    switch (container->GetXPosition())
    {
    case 0:
        posx = xleft;
        break;
    case 1:
        posx = xmiddle;
        break;
    case 2:
        posx = xright;
        break;
    }
    switch (container->GetYPosition())
    {
    case 0:
        posy = ytop;
        break;
    case 1:
        posy = ymiddle;
        break;
    case 2:
        posy = ybottom;
        break;
    }
    grabArm->DropContainer(posx, posy);
    printf(" --- Dropped Container --- \n");
    UpdateState("Idle", "Idle");
    if (*count < 2)
    {
        currentSystemState = IDLE;
        *count += 1;
    }
    else
    {
        *count = 0;
        currentSystemState = CALIBRATING;
    }
}
int main(void)
{
    try
    {
        Setup();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    std::string command;
    Block blockColor = empty;
    Container *container = nullptr;
    bool waiting_for_message_flag = true;
    bool idle_flag = true;
    uint8_t count = 0;
    UpdateStorage();
    while (true)
    {
        switch (currentSystemState)
        {
        case CALIBRATING:
        {
            HandleCalibrateState();
        }
        break;
        case IDLE:
        {
            HandleIdleState(&blockColor, &command, &idle_flag);
        }
        break;
        case SEARCHING_CONTAINER:
        {
            idle_flag = true;
            HandleSearchingState(&blockColor, &command, &container);
            if (currentSystemState == WAITING_FOR_MESSAGE)
            {
                GrabbingContainer(container);
            }
        }
        break;
        case WAITING_FOR_MESSAGE:
        {
            HandleWaitingForMessageState(&blockColor, &command, &container, &waiting_for_message_flag);
            if (waiting_for_message_flag == true)
            {
                DroppingContainer(container, &count);
            }
        }
        break;
        }
    }
    return 0;
    gpioTerminate();
}
