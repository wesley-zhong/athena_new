#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_
#include "ProtoInner.pb.h"
#include "transport/Channel.h"
class Player;

struct InnerLogin {
    InnerLoginRequest req;
    Channel *channel;
};

class MsgHandler {
public:
    static void onShakHand(int64_t playerId, InnerServerHandShakeReq *req);

    static void onInnerLogin(int64_t playerId, InnerLogin *req);
};

#endif
