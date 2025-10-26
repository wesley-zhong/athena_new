//
// Created by zhongweiqi on 2025/10/23.
//

#ifndef ATHENA_CHANNEL_H
#define ATHENA_CHANNEL_H

#include "uv.h"
#include "ByteBuffer.h"

class EventLoop;

class Channel {
public:
    Channel(EventLoop *event_loop, uv_tcp_t *client, uv_os_fd_t fd) : _eventLoop(event_loop),
                                                                      client(client), fd((uint64) fd) {
    }
    void onRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);

    uint64 getFd() const {
        return fd;
    }

    EventLoop *event_loop() {
        return _eventLoop;
    }

    std::string getAddr() const;

private:
    void send(void *data, size_t size);

    ByteBuffer *recv_buffer;
    ByteBuffer *send_buff;
    EventLoop *_eventLoop;
    uv_tcp_t *client;
    uint64 fd;
    bool writing; // whether a uv_write is in-flight
    bool closed; // connection closed
};


#endif //ATHENA_CHANNEL_H
