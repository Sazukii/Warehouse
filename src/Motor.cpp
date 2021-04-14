#include "Motor.h"
#include <iostream>

Motor::Motor(uint8_t pinFWD, uint8_t pinBWD, uint8_t *byDigitalOut)
{
    if (pinFWD != 0 && pinBWD != 0 && byDigitalOut != nullptr)
    {
        this->pinFWD = pinFWD;
        this->pinBWD = pinBWD;
        this->byDigitalOut = byDigitalOut;
    }
    else
    {
        throw std::invalid_argument("Missing a parameter in a motor");
    }
    
}
Motor::~Motor()
{
}

void Motor::Forward()
{
    *byDigitalOut |= pinFWD;  //pinFWD on
    *byDigitalOut &= ~pinBWD; //pinBWD off
}

void Motor::Backward()
{
    *byDigitalOut &= ~pinFWD; //pinFWD off
    *byDigitalOut |= pinBWD;  //pinBWD on
}
void Motor::Stop()
{
    *byDigitalOut &= ~pinBWD; //pinBWD off
    *byDigitalOut &= ~pinFWD; //pinFWD off
}