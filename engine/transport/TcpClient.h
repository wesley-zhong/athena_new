//
// Created by zhongweiqi on 2025/10/27.
//

#ifndef ATHENA_TCPCLIENT_H
#define ATHENA_TCPCLIENT_H
#include <string>
#include "EventLoop.h"
#include "NetInterface.h"


class TcpClient : public NetInterface {
public:
    TcpClient();

    ~TcpClient() {
    }
    void start();
    void on_new_connection(Channel *channel) {}

    void connect(const std::string &ip, int port);

    void on_connected(Channel *channel) override {
        if (onConnected != nullptr) {
            onConnected(channel);
        }
    }

    void on_read(Channel *channel, char *body, int len) override {
        if (onRead != nullptr) {
            onRead(channel, body, len);
        }
    }

    void on_closed(Channel *channel) override {
        if (onClosed != nullptr) {
            onClosed(channel);
        }
    }

    std::function<void(Channel *)> onConnected;
    std::function<void(Channel *, char *, int)> onRead;
    std::function<void(Channel *)> onClosed;

private:
    EventLoop *loop;
};


#endif //ATHENA_TCPCLIENT_H
