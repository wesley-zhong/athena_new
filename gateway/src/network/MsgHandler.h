#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_

#include "BaseType.h"
#include "ProtoInner.pb.h"
#include "transport/Channel.h"

struct InnerLogin {
    InnerLoginRequest req;
    Channel *channel;
};

class MsgHandler {
public:
    static void onLoginRes(int64_t playerId, InnerLoginResponse *res);

    static void onShakeHandRes(int64_t playerId, InnerServerHandShakeRes *res);

   // static onHeartBeatRes(int64 playerId, R)
};

#endif
