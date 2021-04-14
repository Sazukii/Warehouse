#include "Conveyor.h"
#include <stdexcept>
Conveyor::Conveyor(IMotor *conveyorMotor, pixtInV2L *InputData, pixtOutV2L *OutputData)
{
    if (conveyorMotor != nullptr && InputData != nullptr && OutputData != nullptr)
    {
        this->InputData = InputData;
        this->OutputData = OutputData;
        this->conveyorMotor = conveyorMotor;
    }
    else
    {
        throw std::invalid_argument("Missing a parameter");
    }
}
Conveyor::~Conveyor()
{
}
void Conveyor::Update()
{
    Spi_AutoModeV2L(OutputData, InputData);
    Spi_AutoModeV2L(OutputData, InputData);
}
void Conveyor::MoveToExit()
{
    conveyorMotor->Forward();
    Update();
}
void Conveyor::MoveToWarehouse()
{
    conveyorMotor->Backward();
    Update();
}
void Conveyor::StopConveyor()
{
    conveyorMotor->Stop();
    Update();
}