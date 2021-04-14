#if !defined(CONVEYOR_H)
#define CONVEYOR_H
#include "Defines.h"
#include "Motor.h"
class Conveyor
{
private:
    IMotor *conveyorMotor = nullptr;
    pixtInV2L *InputData = nullptr;
    pixtOutV2L *OutputData = nullptr;
    void Update();
public:
    Conveyor(IMotor *conveyorMotor, pixtInV2L *InputData, pixtOutV2L *OutputData);
    ~Conveyor();
    void MoveToExit();
    void MoveToWarehouse();
    void StopConveyor();
};


#endif // CONVEYOR_H
