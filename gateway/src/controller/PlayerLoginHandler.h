#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_


#include "ProtoInner.pb.h"
#include "transport/Channel.h"

class PlayerLoginHandler {
public:
    static void registMsgHandler();

private:
    static void onLoginRes(Channel *channel, InnerLoginResponse *res);
};

#endif
