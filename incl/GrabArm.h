#ifndef GRABARM_H
#define GRABARM_H

#include "Encoder.h"
#include "Motor.h"

class GrabArm
{
private:
    uint32_t axisX;
    uint32_t axisY;
    struct pixtInV2L *InputData;
    struct pixtOutV2L *OutputData;
    bool calibratedHorizontal = false;
    bool calibratedVertical = false;
    bool calibratedArm = false;
    IMotor *horizontalMotor = nullptr;
    IEncoder *horizontalEncoder = nullptr;
    IMotor *verticalMotor = nullptr;
    IEncoder *verticalEncoder = nullptr;
    IMotor *armMotor = nullptr;
    void RetractArm();
    void ExtendArm();
    void Update();
    void MoveVertical(int verticalSteps);
    void MoveHorizontal(int horizontalSteps);
    static void StaticMoveHorizontal(GrabArm *grabarm, int horizontalSteps);
    static void StaticMoveVertical(GrabArm *grabarm, int verticalSteps);

public:
    GrabArm(pixtInV2L *InputData, pixtOutV2L *OutputData, IMotor *horizontalMotor, IEncoder *horizontalEncoder, IMotor *verticalMotor, IEncoder *verticalEncoder, IMotor *armMotor);
    ~GrabArm();

    void DropContainer(uint32_t x, uint32_t y);
    void GrabContainer(uint32_t x, uint32_t y);
    void MoveTo(uint32_t x, uint32_t y);
    void Calibrate();
};
#endif