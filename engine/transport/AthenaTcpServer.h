//
// Created by zhongweiqi on 2025/10/23.
//

#ifndef ATHENA_TCPSERVER_H
#define ATHENA_TCPSERVER_H
#include <atomic>
#include <memory>
#include <vector>
#include "EventLoop.h"

#include "NetInterface.h"
#include "Channel.h"

class AthenaTcpServer : public NetInterface {
public:
    AthenaTcpServer() {
    }

    ~AthenaTcpServer() {
    }


    AthenaTcpServer &bind(int port);

    void start(int eventLoopNum);

    void stop();

    void onAccept(uv_stream_t *server, int status);

    void on_connected(Channel *channel) override {
    }

    void on_new_connection(Channel *channel) override {
        if (onNewConnection != nullptr) {
            onNewConnection(channel);
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

    std::function<void(Channel *)> onNewConnection;
    std::function<void(Channel *, char *, int)> onRead;
    std::function<void(Channel *)> onClosed;

    static void uv_on_new_connection(uv_stream_t *server, int status);

private:
    std::atomic<size_t> next_reactor{0};
    std::vector<std::unique_ptr<EventLoop> > event_loops_;
    uv_loop_t main_loop;
    uv_tcp_t server;
};


#endif //ATHENA_TCPSERVER_H
