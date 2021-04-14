#ifndef MOTOR_H
#define MOTOR_H
#include "IMotor.h"
#include "Defines.h"
class Motor : public IMotor
{
private:
    uint8_t pinFWD;
    uint8_t pinBWD;
    uint8_t* byDigitalOut;

public:
    Motor(uint8_t pinFWD, uint8_t pinBWD, uint8_t* byDigitalOut);
    ~Motor();
    void Forward();
    void Backward();
    void Stop();
};

#endif
