//
// Created by zhongweiqi on 2025/10/23.
//

#ifndef ATHENA_TCPSERVER_H
#define ATHENA_TCPSERVER_H
#include <atomic>
#include <memory>
#include <vector>
#include "EventLoop.h"

#include "uv.h"

class AthenaTcpServer {
public:
    AthenaTcpServer(){};

    AthenaTcpServer &bind(int port);

    void start(int eventLoopNum);

    void stop();

    void onAccept(uv_stream_t *server, int status);

    ~AthenaTcpServer(){};

    static void uv_on_new_connection(uv_stream_t *server, int status);

private:
    std::atomic<size_t> next_reactor{0};
    std::vector<std::unique_ptr<EventLoop> > event_loops_;
    uv_loop_t main_loop;
    uv_tcp_t server;
};


#endif //ATHENA_TCPSERVER_H
