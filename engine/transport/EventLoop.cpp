//
// Created by zhongweiqi on 2025/10/23.
//

#include "EventLoop.h"
#include "XLog.h"
#include <string>
#include "AthenaTcpServer.h"

void async_accept_cb(uv_async_t *handle) {
    EventLoop *event_loop = (EventLoop *) handle->data;
    event_loop->execute();
}

void async_connect_cb(uv_async_t *handle) {
    EventLoop *event_loop = (EventLoop *) handle->data;
    event_loop->execute();
}

void async_write_cb(uv_async_t *handle) {
    EventLoop *event_loop = (EventLoop *) handle->data;
    event_loop->execute();
}

void EventLoop::uv_alloc_cb(uv_handle_t *h, size_t s, uv_buf_t *buf) {
    buf->base = (char *) malloc(s);
    buf->len = s;
}

void EventLoop::uv_on_connect(uv_connect_t *req, int status) {
    if (status < 0) {
        INFO_LOG("xxxxxxxxxxxxxx CONNECT FAILED");
        // this should free channel
        return;
    }
    Channel *channel = (Channel *) req->data;
    EventLoop *event_loop = channel->event_loop();
    uv_read_start(req->handle, uv_alloc_cb, uv_read_cb);
    event_loop->_netInterface->on_connected(channel);
}


void EventLoop::uv_read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    Channel *channel = (Channel *) client->data;
    channel->onRead(client, nread, buf);
    free(buf->base);
    EventLoop *event_loop = channel->event_loop();
    // get one complete packet
    while (true) {
        int packageLen = channel->getPack(event_loop->maxPackBody);
        if (packageLen < 0) {
            return;
        }
        event_loop->_netInterface->on_read(channel, event_loop->maxPackBody, packageLen);
    }
}

void EventLoop::uv_on_timer(uv_timer_t *timer) {
    Channel *channel = (Channel *) timer->data;

    uint64_t now = uv_now(channel->event_loop()->uv_loop());

    if (now - channel->last_recv_time > 15000) {
        // 超过15秒无数据
        INFO_LOG("Client timeout, closing...\n");
        uv_close((uv_handle_t *) (channel->client), [](uv_handle_t *h) {
            delete h;
        });
        uv_timer_stop(timer);
        return;
    }

    // // 发送心跳包
    // const char *msg = "PING";
    // uv_buf_t buf = uv_buf_init((char *) msg, strlen(msg));
    // uv_write_t *req = new uv_write_t;
    // uv_write(req, (uv_stream_t *) &c->handle, &buf, 1, [](uv_write_t *req, int status) {
    //     delete req;
    // });
}

void EventLoop::asyncConnect(const std::string &ip, int port) {
    EventLoop *event_loop = this;
    push([ip, port, event_loop]() {
        sockaddr_in dest;
        int ret = uv_ip4_addr(ip.c_str(), port, &dest);
        if (ret != 0) {
            ERR_LOG("uv_ip4_addr failed code =code {}:{}", ret, uv_strerror(ret));
            return;
        }
        uv_connect_t *connect_req = new uv_connect_t;
        uv_tcp_t *client = new uv_tcp_t;
        uv_tcp_init(event_loop->uv_loop(), client);
        Channel *clientChannel = new Channel(event_loop, client, 1);
        connect_req->data = clientChannel;
        client->data = clientChannel;
        ret = uv_tcp_connect(connect_req, client, (const struct sockaddr *) &dest, uv_on_connect);
        if (ret != 0) {
            ERR_LOG("uv_tcp_connect  failed code =code {}:{}", ret, uv_strerror(ret));
        }
    });

    async_connect_task();
}

void EventLoop::asyncAccept(uv_os_sock_t fd) {
    EventLoop *event_loop = this;
    push([event_loop, fd] {
        uv_tcp_t *client = new uv_tcp_t;
        uv_tcp_init(event_loop->uv_loop(), client);
        int ret = uv_tcp_open(client, fd); // 绑定 socket 到本 loop
        if (ret != 0) {
            ERR_LOG("  -------tcp open faild ret ={}", ret);
        }

        uv_read_start((uv_stream_t *) client, uv_alloc_cb, uv_read_cb);
        Channel *channel = new Channel(event_loop, client, fd);
        client->data = channel;
        event_loop->onNewConnection(channel);
        INFO_LOG(" =================== START READ read data ");
    });
    async_accept_task();
}

void EventLoop::onNewConnection(Channel *channel) const {
    _netInterface->on_new_connection(channel);
}

void EventLoop::onClosed(Channel *channel) const {
    _netInterface->on_closed(channel);
}

void EventLoop::onRead(Channel *channel, char *body, int len) const {
    _netInterface->on_read(channel, body, len);
}


void EventLoop::run() {
    _loop = new uv_loop_t;
    uv_loop_init(_loop);
    // store reactor pointer in loop->data for later retrieval in read callbacks
    _loop->data = this;

    int ret = uv_async_init(_loop, &uv_async_write, async_write_cb);
    if (ret != 0) {
        ERR_LOG("uv_async_init  async_write  ret ={} ", ret);
        return;
    }
    uv_async_write.data = this;
    ret = uv_async_init(_loop, &uv_async_accept, async_accept_cb);
    if (ret != 0) {
        ERR_LOG("uv_async_init  async_accept ret ={} ", ret);
        return;
    }
    uv_async_accept.data = this;

    ret = uv_async_init(_loop, &uv_async_connect, async_connect_cb);
    if (ret != 0) {
        ERR_LOG("uv_async_init  async_accept ret ={} ", ret);
        return;
    }
    uv_async_connect.data = this;
    INFO_LOG("############## event loop started");
    uv_run(_loop, UV_RUN_DEFAULT);
    uv_close(reinterpret_cast<uv_handle_t *>(&uv_async_write), nullptr);
    uv_close(reinterpret_cast<uv_handle_t *>(&uv_async_accept), nullptr);
    uv_close(reinterpret_cast<uv_handle_t *>(&uv_async_connect), nullptr);

    uv_loop_close(_loop);
    delete _loop;
    _loop = nullptr;
    INFO_LOG(" event  run end ");
}

void EventLoop::execute() {
    while (true) {
        Thread::TaskPtr task_ptr = pop();
        if (task_ptr == nullptr) {
            return;
        }
        task_ptr->run();
    }
}


void EventLoop::start() {
    t = std::thread(&EventLoop::run, this);
}


void EventLoop::startHeartbeatTimer(Channel *channel) {
    uv_timer_init(this->uv_loop(), channel->getTimer());
    channel->getTimer()->data = channel;
    uv_timer_start(channel->getTimer(), uv_on_timer, 5000, 0);
}
