#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <unistd.h>
#include <thread>
#include <arpa/inet.h>
#include "thread_pool.h"

class TCPServer {
public:
    TCPServer(const uint32_t port, size_t thread_pool_size = MAX_THREADS_NUM);
    virtual ~TCPServer();
public:
    bool Start();
    void Close();
private:
    uint32_t port_;
    int fd_;
    std::thread listen_thread_;
    ThreadPool thread_pool_;
public:
    static bool stop_;
};

#endif //_TCP_SERVER_H_