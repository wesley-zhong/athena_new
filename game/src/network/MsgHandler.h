#ifndef MSGHANDLER_H_
#define MSGHANDLER_H_
#include "ProtoInner.pb.h"
#include "transport/Channel.h"
class Player;

template<class T>
struct ReqChannel {
    Channel *channel;
    T req;

    ~ReqChannel() {
        channel = nullptr;
        req.Clear();
    }
};

class MsgHandler {
public:
    static void onShakHandReq(Channel *channel, InnerServerHandShakeReq *req);

    static void onHeartBeat(Channel *channel, InnerHeartBeatRequest *req);

    static void onInnerLogin(Channel *channel, InnerLoginRequest *req);
};

#endif
