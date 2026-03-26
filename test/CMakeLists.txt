find_package(GTest REQUIRED)

add_executable(tcp-client-test
    test_tcp.cpp
)

target_include_directories(tcp-client-test PRIVATE
    ${CMAKE_SOURCE_DIR}/output/inc
)

target_link_directories(tcp-client-test PRIVATE
    ${CMAKE_SOURCE_DIR}/output/lib
)

target_link_libraries(tcp-client-test PRIVATE
    GTest::GTest
    tcp-client
)

include(GoogleTest)
gtest_discover_tests(tcp-client-test)
