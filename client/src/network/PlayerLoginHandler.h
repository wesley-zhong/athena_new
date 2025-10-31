#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_

#include "BaseType.h"
#include "ProtoInner.pb.h"
#include "transport/Channel.h"

struct InnerLogin {
    InnerLoginRequest req;
    Channel *channel;
};

class PlayerLoginHandler {
public:
    static void onLoginRes(Channel *channel, InnerLoginResponse *res);

    static void onShakeHandRes(Channel *channel, InnerServerHandShakeRes *res);
};

#endif
