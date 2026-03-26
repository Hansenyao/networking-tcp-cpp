#include <gtest/gtest.h>
#include "tcp_client.h"

/**
 * @brief Test cases for TCP client
 * 
 * Need to run the TCP server (./build/src/server/tcp-server) in the local before running this test!
 * 
 */
const char* VALID_SVR_IP = "127.0.0.1";
const int VALID_SVR_PORT = 8088;

TEST(TCPClientTest, ConnectValidServerAndSendMsgSuccess) {
    TCPClient client;
    std::string response;
    const char* msg = "A message comes from client";

    EXPECT_TRUE(client.Connect(VALID_SVR_IP, VALID_SVR_PORT));
    
    EXPECT_TRUE(client.SendMessage(msg, response));

    // The server's response string should include original string
    EXPECT_TRUE(response.find(msg) != std::string::npos);   

    EXPECT_NO_THROW(client.Disconnect());
}

TEST(TCPClientTest, ConnectInvalidIp) {
    TCPClient client;
    // IP string too short (<=6 chars triggers early rejection)
    EXPECT_FALSE(client.Connect("1.1.1", 8088));
}

TEST(TCPClientTest, ConnectInvalidPort) {
    TCPClient client;
    EXPECT_FALSE(client.Connect("127.0.0.1", 0));
    EXPECT_FALSE(client.Connect("127.0.0.1", -1));
}

TEST(TCPClientTest, ConnectRefused) {
    TCPClient client;
    // Port 19999 assumed unused; connect should fail gracefully
    EXPECT_FALSE(client.Connect("127.0.0.1", 19999));
}

TEST(TCPClientTest, SendMessageWithoutConnect) {
    TCPClient client;
    std::string resp;
    // Calling SendMessage before Connect should return false
    EXPECT_FALSE(client.SendMessage("hello", resp));
}

TEST(TCPClientTest, SendEmptyMessage) {
    TCPClient client;
    // Even if connected, empty message should be rejected
    // (We cannot connect here, so we only verify the guard when fd_ < 0)
    std::string resp;
    EXPECT_FALSE(client.SendMessage("", resp));
}

TEST(TCPClientTest, DisconnectWithoutConnect) {
    TCPClient client;
    // Calling Disconnect on a never-connected client should not crash
    EXPECT_NO_THROW(client.Disconnect());
}

TEST(TCPClientTest, DisconnectTwice) {
    TCPClient client;
    // Double Disconnect should be safe
    EXPECT_NO_THROW({
        client.Disconnect();
        client.Disconnect();
    });
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
