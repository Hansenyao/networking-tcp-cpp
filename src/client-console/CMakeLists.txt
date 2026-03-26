add_executable(tcp-client-console
    main.cpp
)

target_link_libraries(tcp-client-console PRIVATE
    -ltcp-client
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../client-lib
)

target_link_directories(tcp-client-console PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/../client-lib
)