#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_
#include "common/Singleton.h"
#include <iostream>

#include "ProtoInner.pb.h"
class GameRole;

class MsgHandler : public Singleton<MsgHandler> {
private:
    /* data */
public:
    MsgHandler(/* args */) {
    };

    ~MsgHandler() {
    };

    static void onLogin(int64_t playerId, GameRole *GameRole);

    static void onSomeMsg(int64_t playerId, InnerHead *InnerHead);

    static void onInnerLogin(int64_t playerId, InnerLoginRequest *GameRole);
};

#endif
