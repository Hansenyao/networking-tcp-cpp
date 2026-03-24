#include "tcp_server.h"
#include <iostream>
#include <csignal>

std::atomic<bool> running(true);

void signal_handler(int) {
    running = false;
}

int main()
{
    // handling Ctrl + C
    signal(SIGINT, signal_handler);

    // start tcp server
    TCPServer server(8088);
    if (!server.Start()) {
        std::cout << "Failed to start TCP server, exit!" << std::endl;
        return -1;
    }

    // main loop, waiting user input Ctrl + C to quit
    std::cout << "Press Ctrl + C to quit..." << std::endl;
    while (running) {
        sleep(1);
    };

    // close tcp server before end
    server.Close();
    std::cout << "End!" << std::endl;

    return 0;
}