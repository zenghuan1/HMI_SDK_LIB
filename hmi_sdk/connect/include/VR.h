﻿#ifndef VRCLIENT_H_
#define VRCLIENT_H_

#include "Channel.h"

class VR : public Channel
{
public:
    VR();
    ~VR();
protected:
    void onRegistered();
    void onUnregistered();

    void onRequest(Json::Value &);

private:

};

#endif // VRCLIENT_H_
