#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_
#include "common/Singleton.h"
#include <iostream>

class GameRole;
class InnerHead;
class MsgHandler :public Singleton<MsgHandler>
{
private:
    /* data */
public:
    MsgHandler(/* args */){};
    ~MsgHandler(){};
    static void  onLogin(int64_t playerId, GameRole* GameRole);
    static void  onSomeMsg(int64_t playerId, InnerHead* InnerHead );
};

#endif