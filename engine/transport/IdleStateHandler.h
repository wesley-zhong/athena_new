//
// Created by zhongweiqi on 2025/10/30.
//

#ifndef ATHENA_IDLESTATEHANDLER_H
#define ATHENA_IDLESTATEHANDLER_H
#include "BaseType.h"
#include "Channel.h"
#include "EventTrigger.h"
class NetInterface;

class IdleStateHandler : public EventTrigger {
public:
    IdleStateHandler(NetInterface *net_interface, uint64 max_write_time,
                     uint64 max_read_time) : netInterface(net_interface), max_write_idle_time(max_write_time),
                                             max_read_idle_time(max_read_time) {
    }

    void triggerEvent(Channel *channel, TriggerEventEnum reason) override;

    void onTimer(Channel *channel, uint64 now) override;

private:
    NetInterface *netInterface;
    uint64 max_write_idle_time;
    uint64 max_read_idle_time;
};


#endif //ATHENA_IDLESTATEHANDLER_H
