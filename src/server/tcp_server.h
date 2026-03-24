#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <unistd.h>
#include <thread>
#include <arpa/inet.h>

class TCPServer {
public:
    TCPServer(const uint32_t port);
    virtual ~TCPServer();
public:
    bool Start();
    void Close();
private:
    uint32_t port_;
    int fd_;
    std::thread listen_thread_;
public:
    static bool stop_;
};

#endif //_TCP_SERVER_H_