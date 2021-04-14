#ifndef IMOTOR_H
#define IMOTOR_H
class IMotor
{
private:
    /* data */
public:
    virtual void Forward() = 0;
    virtual void Backward() = 0;
    virtual void Stop() = 0;
};

#endif