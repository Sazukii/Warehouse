#include "Encoder.h"
#include <pigpio.h>

Encoder::Encoder(uint8_t encoderPin1, uint8_t encoderPin2)
{
    if (encoderPin1 != 0 && encoderPin2 != 0)
    {
        this->encoderPin1 = encoderPin1;
        this->encoderPin2 = encoderPin2;
        
        gpioSetMode(encoderPin1, PI_INPUT);
        gpioSetMode(encoderPin2, PI_INPUT);
    }
    else
    {
        throw std::invalid_argument("Missing a parameter in an encoder");
    }
}
Encoder::~Encoder()
{
}

void Encoder::CheckRotation(int ticks, bool FWD)
{
    printf(" --- Checking Rotation --- \n");

    int count = 0;
    uint8_t encoderPulse1 = 0;
    uint8_t encoderPulse2 = 0;
    uint8_t encoderFlag = 0;

    while (count < ticks)
    {
        encoderPulse1 = gpioRead(encoderPin1);
        encoderPulse2 = gpioRead(encoderPin2);
        if (encoderPulse1 == 1 && FWD == 1 && encoderFlag == 0) //++
        {
            if (encoderPulse2 == 0)
            {
                count++;
                encoderFlag = 1;
            }
        }
        else if (encoderPulse1 == 1 && FWD == 0 && encoderFlag == 0) //--
        {
            if (encoderPulse2 == 1)
            {
                count++;
                encoderFlag = 1;
            }
        }

        if (encoderPulse1 == 0)
        {
            encoderFlag = 0;
        }
    }
}