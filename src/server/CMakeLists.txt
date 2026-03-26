add_executable(tcp-server
    main.cpp
    tcp_server.cpp
    thread_pool.cpp
)

target_link_libraries(tcp-server PRIVATE pthread)