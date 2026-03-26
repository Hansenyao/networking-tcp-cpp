#include "tcp_server.h"
#include <iostream>
#include <fcntl.h>
#include <cstring>

const int BUFFER_SIZE = 2048;
bool TCPServer::stop_  = false;

// Client request handling thread
void client_handle_thread(int client_fd)
{
    char buffer[BUFFER_SIZE] = { 0 };

    while(true) {
        memset(buffer, 0, BUFFER_SIZE);

        int bytes = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes > 0 ) {
            std::string response = "Server recieved: ";
            response.append(buffer, bytes);
            send(client_fd, response.c_str(), response.length(), 0);
        }
        else if (bytes == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        else { // bytes < 0
            if (errno == EINTR) {
                // Interrupted by signal
                continue;
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data, try it again in a short later
                usleep(1000);
                continue;
            }
            else {
                perror("recv failed");
                break;
            }
        }
    }
}

// Server listen thread
void server_accept_thread(int server_fd, ThreadPool* pool)
{
    while (!TCPServer::stop_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd > 0) {
            std::cout << "New client connected, ip: " << inet_ntoa(client_addr.sin_addr) << std::endl;

            // submit client handling to thread pool
            pool->AddTask(client_handle_thread, client_fd);
        }

        // 50ms
        usleep(50*1000);
    }
}

TCPServer::TCPServer(const uint32_t port, size_t thread_pool_size)
 : fd_(-1)
 , port_(port)
 , thread_pool_(thread_pool_size)
{
}

TCPServer::~TCPServer() 
{
    if (fd_ > 0) {
        Close();
    }
};

bool TCPServer::Start()
{
    if (fd_ > 0) {
        perror("server is running already");
        return false;
    }

    // Create socket with IPv4 and TCP
    if ((fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("create socket failed");
        return false;
    }

    // Set re-use for port if its status is TIME_WAIT
    int opt = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Set No-Block
    int flags = fcntl(fd_, F_GETFL, 0);
    fcntl(fd_, F_SETFL, flags | O_NONBLOCK);

    // Configure address
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = INADDR_ANY;

    // bind address
    if (bind(fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind address failed");
        return false;
    }

    // listen, set the maximun accept queue is 10
    if (listen(fd_, 10) < 0) {
        perror("listen failed");
        return false;
    }

    // create a thread to listen clients requests
    listen_thread_ = std::thread(server_accept_thread, fd_, &thread_pool_);
    return true;
}

void TCPServer::Close()
{
    // Waiting for listen thread to end
    stop_ = true;
    listen_thread_.join();

    // Shutdown thread pool
    thread_pool_.Stop();

    // Close socket
    close(fd_);
    fd_ = -1;
}