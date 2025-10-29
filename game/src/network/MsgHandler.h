#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_
#include "ProtoInner.pb.h"
#include "transport/Channel.h"
class Player;

struct InnerLogin {
    InnerLoginRequest req;
    Channel *channel;
};

struct InnerShakeHand {
    InnerServerHandShakeReq req;
    Channel *channel;
};

class MsgHandler {
public:
    static void onShakHandReq(int64_t playerId, InnerShakeHand *req);

    static void onInnerLogin(int64_t playerId, InnerLogin *req);
};

#endif
