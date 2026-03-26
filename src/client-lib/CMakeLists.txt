add_library(tcp-client SHARED
    tcp_client.cpp
)

target_include_directories(tcp-client PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

install(TARGETS tcp-client
    LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/output/lib
)

install(FILES ${CMAKE_SOURCE_DIR}/src/client-lib/tcp_client.h
    DESTINATION ${CMAKE_SOURCE_DIR}/output/inc
)
