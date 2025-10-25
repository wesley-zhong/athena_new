//
// Created by zhongweiqi on 2025/10/23.
//

#include "EventLoop.h"
#include "XLog.h"

void async_accept_cb(uv_async_t *handle) {
    EventLoop *event_loop = (EventLoop *) handle->data;
    event_loop->execute();
}

void EventLoop::uv_alloc_cb(uv_handle_t *h, size_t s, uv_buf_t *buf) {
    buf->base = new char[s];
    buf->len = s;
}


void EventLoop::uv_read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    EventLoop *event_loop = (EventLoop *) client->data;;
    event_loop->onRead(client, nread, buf);


    // EventLoop *pEventPool = (EventLoop *) client->data;
    // if (nread > 0) {
    //     std::string msg(buf->base, nread);
    //     INFO_LOG("-----------read msg ={}", msg);
    //     // 投递到逻辑线程
    //     // g_logicPool.post([client, msg] {
    //     //     std::string reply = "echo: " + msg;
    //     //     // 回到对应 Reactor 写回
    //     //     auto *r = (ReactorThread *) client->data;
    //     //     r->q.push([client, reply] {
    //     //         auto *req = new uv_write_t;
    //     //         auto *data = new std::string(reply);
    //     //         uv_buf_t b = uv_buf_init(data->data(), data->size());
    //     //         req->data = data;
    //     //         uv_write(req, (uv_stream_t *) client, &b, 1,
    //     //                  [](uv_write_t *req, int status) {
    //     //                      delete (std::string *) req->data;
    //     //                      delete req;
    //     //                  });
    //     //     });
    //     //     uv_async_send(&r->async);
    //     // });
    // } else {
    //     uv_close((uv_handle_t *) client, nullptr);
    // }
    // delete[] buf->base;
}


void async_write_cb(uv_async_t *handle) {
    EventLoop *event_loop = (EventLoop *) handle->data;
}

void EventLoop::asyncAccept(uv_stream_t *server, uv_tcp_t *client) {
    push([server, client] {
        if (uv_accept(server, (uv_stream_t *) client) != 0) {
            uv_close((uv_handle_t *) client, [](uv_handle_t *h) { free(h); });
            return;
        }
        uv_os_fd_t fd;
        uv_fileno((const uv_handle_t *) client, &fd);
        uv_read_start((uv_stream_t *) client, uv_alloc_cb, uv_read_cb);
        INFO_LOG(" =================== START READ read data ");
    });
    async_accept_task();
}

void EventLoop::run() {
    INFO_LOG(" event  run start ");
    _loop = new uv_loop_t;
    uv_loop_init(_loop);
    // store reactor pointer in loop->data for later retrieval in read callbacks
    _loop->data = this;

    int ret = uv_async_init(_loop, &uv_async_write, async_write_cb);
    uv_async_write.data = this;
    INFO_LOG("uv_async_init  async_write  ret ={} ", ret);
    ret = uv_async_init(_loop, &uv_async_accept, async_accept_cb);
    uv_async_accept.data = this;
    INFO_LOG("uv_async_init  async_accept ret ={} ", ret);

    uv_run(_loop, UV_RUN_DEFAULT);
    uv_close((uv_handle_t *) &uv_async_write, nullptr);
    uv_close((uv_handle_t *) &uv_async_accept, nullptr);

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


void EventLoop::onRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        std::string msg(buf->base, nread);
        INFO_LOG("-----------read msg ={}", msg);
        delete[] buf->base;
        return;
    }
    INFO_LOG("socket closed =");
    uv_close((uv_handle_t *) client, nullptr);
}

