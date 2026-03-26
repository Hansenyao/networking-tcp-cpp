#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <string>

class TCPClient
{
public:
    TCPClient();
    virtual ~TCPClient();
public:
    bool Connect(const std::string & svr_ip, int svr_port);
    bool SendMessage(const std::string & msg, std::string & resp);
    void Disconnect();
private:
    int fd_;
};

#endif //_TCP_CLIENT_H_