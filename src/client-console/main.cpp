#include "tcp_client.h"
#include <iostream>
#include <unistd.h>
#include <string>

int main(int argc, char** arg)
{
    if (argc < 3) {
        std::cout << "Invalid parameters, please input server ip and port. Exit!" << std::endl;
        return -1;
    }

    const char* svr_ip = arg[1];
    int svr_port = atoi(arg[2]);

    TCPClient client;

    // Connet to the server
    if (!client.Connect(svr_ip, svr_port)) {
        std::cout << "Failed to connect (" << svr_ip << ":" << svr_port 
                  << "), error: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Connect to (IP: " << svr_ip << ": " << svr_port << ") successfully!" << std::endl;

    // Send message to server and receive the response message
    int times = 0;
    const std::string msg_base = "This is a message comes from client! Current Times: ";
    while(true) {
        std::string response;
        std::string msg = msg_base + std::to_string(++times);

        if (!client.SendMessage(msg, response)) {
            std::cout << "Failed to send message, error: " << strerror(errno) << std::endl;
            return -1;
        }
        
        std::cout << "Sent: " << msg << std::endl;
        std::cout << "Recv: " << response << std::endl;

        sleep(1);
    }

    return 0;
}