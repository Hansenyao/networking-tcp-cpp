add_library(tcp-client SHARED
    tcp_client.cpp
)

install(TARGETS tcp-client
    LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/output/lib
)

install(FILES ${CMAKE_SOURCE_DIR}/src/client-lib/tcp_client.h
    DESTINATION ${CMAKE_SOURCE_DIR}/output/inc
)
