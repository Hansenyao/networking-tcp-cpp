# networking-tcp-cpp

A TCP server and client implementation in C++17.

## Project Structure

```
networking-tcp-cpp/
├── src/
│   ├── server/          # TCP server (executable)
│   │   ├── tcp_server.h/cpp
│   │   ├── thread_pool.h/cpp
│   │   └── main.cpp
│   ├── client-lib/      # TCP client (shared library)
│   │   └── tcp_client.cpp
│   └── client-console/  # TCP client console (executable)
│       └── main.cpp
├── test/                # GTest unit tests for tcp-client
│   └── test_tcp.cpp
└── output/
    ├── inc/             # Installed headers
    └── lib/             # Installed shared library
```

## Features

- **TCPServer**: accepts multiple concurrent client connections, each handled by a worker thread drawn from a fixed-size thread pool. Responds to every message with `"Server recieved: <message>"`. Supports graceful shutdown via `Ctrl+C`.
- **TCPClient**: shared library exposing `Connect`, `SendMessage`, and `Disconnect`.
- **client-console**: interactive console that connects to the server and sends a message every second. Press `Q` to quit.

## Dependencies

> This project is primarily developed and tested on **Linux**. macOS is also supported.

| Dependency | Version | Purpose |
|------------|---------|---------|
| CMake | >= 3.20 | Build system |
| g++ | C++17 | Compiler |
| GTest | any | Unit testing (only needed when `BUILD_TESTS=ON`) |

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y cmake g++ libgtest-dev
```

### CentOS / RHEL 8+

```bash
sudo dnf install -y cmake gcc-c++ gtest-devel
```

### CentOS 7

```bash
sudo yum install -y gcc-c++ gtest-devel
pip3 install cmake --upgrade
```

### macOS

```bash
brew install cmake googletest
```

## Build

```bash
# Configure
$ mkdir build
$ cd build
$ cmake ..

# Build all targets
$ make
```

### Install tcp-client library and header

```bash
$ make install
```

This installs `libtcp-client.so` (Linux) / `libtcp-client.dylib` (macOS) to `output/lib/` and `tcp_client.h` to `output/inc/`.

## Run

### Start the server

```bash
./src/server/tcp-server
```

The server listens on port **8088** by default. Press `Ctrl+C` to stop.

### Start the client console

```bash
./src/client-console/tcp-client-console <server_ip> <port>

# Example
./src/client-console/tcp-client-console 127.0.0.1 8088
```

Press `Q` to disconnect and exit.

## Testing

### Build with tests enabled

```bash
$ cmake .. -DBUILD_TESTS=ON
$ make
```

### Run tests

> The server must be running on `127.0.0.1:8088` before running the tests.

```bash
# Start the server first
$ ./src/server/tcp-server

# Run all test cases
$ ctest
```

Expected output:

```
100% tests passed, 0 tests failed out of 8
```

## Future Improvements

- **High-concurrency I/O model**: replace the current one-thread-per-connection model with `select` / `poll` (cross-platform) or `epoll` (Linux) / `kqueue` (macOS) to handle a large number of simultaneous connections with far fewer threads.
- **Application-level protocol**: define a structured message format (e.g., a fixed header containing message type, payload length, and sequence number) so that both sides can reliably frame, parse, and route messages without ambiguity.
- **Serialization format**: adopt a standard data serialization format such as JSON, Protobuf, or MessagePack to make the payload extensible and interoperable with other languages/platforms.
- **Configurable server parameters**: expose port, thread pool size, and backlog queue length via a config file or command-line arguments instead of hardcoding them.
- **Heartbeat / keep-alive**: implement a periodic heartbeat mechanism so that the server can detect and clean up silently disconnected clients in a timely manner.
- **TLS encryption**: wrap the socket layer with OpenSSL or mbedTLS to secure data in transit.
- **Logging**: integrate a structured logging library (e.g., spdlog) to replace bare `std::cout` / `perror` calls, with log levels and file rotation support.
