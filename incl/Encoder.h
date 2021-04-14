#ifndef ENCODER_H
#define ENCODER_H

#include "IEncoder.h"
#include "Defines.h"

class Encoder : public IEncoder
{
private:
    uint8_t encoderPin1;
    uint8_t encoderPin2;

public:
    Encoder(uint8_t encoderPin1, uint8_t encoderPin2);
    ~Encoder();
    void CheckRotation(int ticks, bool FWD);
};

#endif