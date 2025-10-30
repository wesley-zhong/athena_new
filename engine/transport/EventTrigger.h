//
// Created by zhongweiqi on 2025/10/30.
//

#ifndef ATHENA_EVENTTRIGGER_H
#define ATHENA_EVENTTRIGGER_H
#include "BaseType.h"
#include "EventDefs.h"
#include "Channel.h"

class EventTrigger {
public:
    virtual void onTimer(Channel *channel, uint64 now) =0;

    virtual void triggerEvent(Channel *channel, TriggerEventEnum reason) = 0;
};


#endif //ATHENA_EVENTTRIGGER_H
