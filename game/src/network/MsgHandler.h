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
    static void onShakHandReq(int64_t playerId, ReqChannel<InnerServerHandShakeReq> *req);
    static void onHeartBeat(int64_t playerId, ReqChannel<InnerServerHandShakeReq> *req);

    static void onInnerLogin(int64_t playerId, ReqChannel<InnerLoginRequest> *req);
};

#endif
