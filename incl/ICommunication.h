#ifndef ICOMMUNICATION_H
#define ICOMMUNICATION_H

class ICommunication
{
public:
    virtual void Setup() = 0;
    virtual char* Receive() = 0;
    virtual void Send(const char* topic, char* payload) = 0;
    virtual void Disconnect() = 0;
};

#endif /* ICOMMUNICATION_H */