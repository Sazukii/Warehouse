#ifndef IENCODER_H
#define IENCODER_H

class IEncoder
{
public:
    virtual void CheckRotation(int ticks, bool FWD) = 0;
};
#endif
