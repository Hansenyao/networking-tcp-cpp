#include "tcp_client.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <thread>

std::atomic<bool> running(true);

#include <termios.h>
#include <unistd.h>

// Get a char from console without echo
char getch() {
    char buf = 0;
    struct termios old{};
    tcgetattr(STDIN_FILENO, &old);

    // change to non-canonical model
    struct termios new_attr = old;
    new_attr.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
    read(STDIN_FILENO, &buf, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    return buf;
}

// Waiting for user input 'Q' to exit
void input_thread() {
    while (true) {
        char c = getch();
        if (c == 'q' || c == 'Q') {
            running = false;
            break;
        }
    }
}

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
    std::cout << "Press 'Q' to exit..." << std::endl << std::endl;
    std::thread t(input_thread);

    // Send message to server and receive the response message
    int times = 0;
    const std::string msg_base = "This is a message comes from client! Current Times: ";
    while(running) {
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

    // Disconnect from the server
    client.Disconnect();

    return 0;
}