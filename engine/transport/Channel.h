//
// Created by zhongweiqi on 2025/10/23.
//

#ifndef ATHENA_CHANNEL_H
#define ATHENA_CHANNEL_H
#include "uv.h"
#include "ByteBuffer.h"

class Channel {
public:
    Channel(uv_tcp_t *client, uv_os_fd_t fd, int idexx) : fd((uint64) fd), client(client) {
    }

    uint64 getFd() const {
        return fd;
    }

    std::string getAddr() const;

private:
    void send(void *data, size_t size);
    ByteBuffer *recv_buffer;
    ByteBuffer *send_buff;
    uv_tcp_t *client;
    uint64 fd;
    void* data;

    bool writing; // whether a uv_write is in-flight
    bool closed; // connection closed
};


#endif //ATHENA_CHANNEL_H
