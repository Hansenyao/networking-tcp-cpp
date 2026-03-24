#include "tcp_client.h"
#include <unistd.h>
#include <arpa/inet.h>

TCPClient::TCPClient()
 : fd_(-1)
{

}

TCPClient::~TCPClient()
{
    if (fd_ > 0) {
        Disconnect();
    }
}

bool TCPClient::Connect(const std::string & svr_ip, int svr_port)
{
    if (svr_ip.size() <= 6 || svr_port <= 0) {
        perror("invalid parameter");
        return false;
    }

    // Disconnect current connection
    if (fd_ > 0) {
        Disconnect();
    }

    // Create socket
    if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create socket failed");
        return false;
    }

    // Set server address
    sockaddr_in svr_addr{};
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(svr_port);
    if (inet_pton(AF_INET, svr_ip.c_str(), &svr_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        return false;
    }

    // Connect server
    if (connect(fd_, (sockaddr*)&svr_addr, sizeof(svr_addr)) < 0) {
        perror("connect failed");
        return false;
    }

    return true;
}

bool TCPClient::SendMessage(const std::string & msg, std::string & resp)
{
    if (fd_ < 0) {
        perror("doesn't connect to server yet");
        return false;
    }

    if (msg.size() == 0) {
        perror("invalid parameter");
        return false;
    }

    // Send message to server
    if (send(fd_, msg.c_str(), msg.size(), 0) < 0) {
        perror("send failed");
        return false;
    }

    // Recieve data from the server
    char buffer[2048] = { 0 };
    int bytes = recv(fd_, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
        resp.append(buffer, bytes);
    }
    else if (bytes == 0) {
        // Server closed the connection
        return false;
    }
    else {
        perror("recv failed");
        return false;
    }

    return true;
}

void TCPClient::Disconnect()
{
    if (fd_ > 0) {
        close(fd_);
        fd_ = -1;
    }
}