#include "tcp_server.h"
#include <iostream>
#include <csignal>

// handling Ctrl + C
volatile sig_atomic_t running = 1;
void signal_handler(int) {
    running = 0;
}

int main()
{
    signal(SIGINT, signal_handler);

    // start tcp server
    TCPServer server(8088);
    if (!server.Start()) {
        std::cout << "Failed to start TCP server, error: " << strerror(errno) << std::endl;
        return -1;
    }

    // main loop, waiting user input Ctrl + C to quit
    std::cout << "Press Ctrl + C to quit..." << std::endl;
    while (running) {
        usleep(100*1000);
    };
    std::cout << std::endl;

    // close tcp server before end
    server.Close();
    std::cout << "End!" << std::endl;

    return 0;
}