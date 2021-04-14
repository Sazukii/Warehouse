#include "GrabArm.h"
#include <iostream>
#include <unistd.h>
#include <thread>

GrabArm::GrabArm(pixtInV2L *InputData,
                 pixtOutV2L *OutputData,
                 IMotor *horizontalMotor,
                 IEncoder *horizontalEncoder,
                 IMotor *verticalMotor,
                 IEncoder *verticalEncoder,
                 IMotor *armMotor)
{
    if (InputData != nullptr && OutputData != nullptr)
    {
        this->InputData = InputData;
        this->OutputData = OutputData;
        calibratedHorizontal = false;
        calibratedVertical = false;
        calibratedArm = false;
    }
    else
    {
        throw std::invalid_argument("Missing a parameter");
    }

    if (horizontalMotor != nullptr && horizontalEncoder != nullptr && verticalMotor != nullptr && verticalEncoder != nullptr && armMotor != nullptr)
    {
        this->horizontalMotor = horizontalMotor;
        this->horizontalEncoder = horizontalEncoder;
        this->verticalMotor = verticalMotor;
        this->verticalEncoder = verticalEncoder;
        this->armMotor = armMotor;
    }
    else
    {
        throw std::invalid_argument("Missing a motor / encoder");
    }
}
GrabArm::~GrabArm()
{
}
void GrabArm::Update()
{
    Spi_AutoModeV2L(OutputData, InputData);
    Spi_AutoModeV2L(OutputData, InputData);
}

void GrabArm::RetractArm()
{
    printf(" --- Arm retracting --- \n");
    armMotor->Backward();
    Update();

    while (!calibratedArm)
    {
        Update();
        if (((InputData->byDigitalIn0 >> BDI0_Ref_Switch_Arm_Back_Pos) & BDI0_Ref_Switch_Arm_Back_Msk) == 1)
        {
            calibratedArm = true;
            armMotor->Stop();
            Update();
        }
    }
    printf(" --- Arm retracted --- \n");
    Update();
}
void GrabArm::ExtendArm()
{
    printf(" --- Arm extending --- \n");

    armMotor->Forward();
    Update();

    while (calibratedArm)
    {
        Update();
        if (((InputData->byDigitalIn0 >> BDI0_Ref_Switch_Arm_Front_Pos) & BDI0_Ref_Switch_Arm_Front_Msk) == 1)
        {
            calibratedArm = false;
            armMotor->Stop();
            Update();
        }
    }
    printf(" --- Arm extended --- \n");
    Update();
}



void GrabArm::DropContainer(uint32_t x, uint32_t y)
{
    MoveTo(x, (y-35));
    ExtendArm();
    MoveTo(x, (y));
    RetractArm();
}

void GrabArm::GrabContainer(uint32_t x, uint32_t y)
{
    MoveTo(x, y);
    ExtendArm();
    MoveTo(x, (y-35));
    RetractArm();
}

void GrabArm::StaticMoveHorizontal(GrabArm *grabarm, int horizontalSteps)
{
    grabarm->MoveHorizontal(horizontalSteps);
}

void GrabArm::MoveHorizontal(int horizontalSteps)
{
    if (horizontalSteps > 0)
    {
        horizontalMotor->Backward();
        Update();
        horizontalEncoder->CheckRotation(horizontalSteps, false);
        horizontalMotor->Stop();
        Update();
        axisX += horizontalSteps;
    }
    else if (horizontalSteps < 0)
    {
        horizontalMotor->Forward();
        Update();
        horizontalEncoder->CheckRotation((horizontalSteps * -1), true);
        horizontalMotor->Stop();
        Update();
        axisX += horizontalSteps;
    }
    
    Update();
}

void GrabArm::StaticMoveVertical(GrabArm *grabarm, int verticalSteps)
{
    grabarm->MoveVertical(verticalSteps);
}

void GrabArm::MoveVertical(int verticalSteps)
{
    if (verticalSteps > 0)
    {
        verticalMotor->Backward();
        Update();
        verticalEncoder->CheckRotation(verticalSteps, false);
        verticalMotor->Stop();
        Update();
        axisY += verticalSteps;
    }
    else if (verticalSteps < 0)
    {
        verticalMotor->Forward();
        Update();
        verticalEncoder->CheckRotation((verticalSteps * -1), true);
        verticalMotor->Stop();
        Update();
        axisY += verticalSteps;
    }
    Update();
}

void GrabArm::MoveTo(uint32_t x, uint32_t y)
{
    printf(" --- Moving to: --- \n");
    printf(" --- x: %d, y: %d --- \n", x, y);
    int horizontalSteps = (x - axisX);
    int verticalSteps = (y - axisY);

    std::thread horizontal(StaticMoveHorizontal, this, horizontalSteps);
    std::thread vertical(StaticMoveVertical, this, verticalSteps);
    
    vertical.join();
    horizontal.join();
}

void GrabArm::Calibrate()
{
    printf(" --- Calibrating Starts --- \n");
    calibratedArm = false;
    RetractArm();

    calibratedHorizontal = false;
    calibratedVertical = false;
    horizontalMotor->Forward(); //to the rack
    verticalMotor->Forward();   //downwards

    Update();

    while (!calibratedHorizontal || !calibratedVertical)
    {
        Update();

        if (((InputData->byDigitalIn0 >> BDI0_Ref_Switch_Horizontal_Pos) & BDI0_Ref_Switch_Horizontal_Msk) == BDI0_Ref_Switch_Horizontal_Msk && calibratedHorizontal == 0)
        {
            horizontalMotor->Stop();
            calibratedHorizontal = 1;
            axisX = 0;
            printf(" --- Calibrated horizontal --- \n");
        }
        if (((InputData->byDigitalIn0 >> BDI0_Ref_Switch_Vertical_Pos) & BDI0_Ref_Switch_Vertical_Msk) == 1 && calibratedVertical == 0)
        {
            verticalMotor->Stop();
            calibratedVertical = 1;
            axisY = 0;
            printf(" --- Calibrated vertical ---\n");
        }
    }
    Spi_AutoModeV2L(OutputData, InputData);
    Update();
}
