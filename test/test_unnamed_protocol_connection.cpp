#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "src/unnamed_protocol/socket_handler.hpp"

namespace unnamed_protocol::test
{
TEST(SocketHandlerShould, SendAndReceiveStringMessagesWithEnoughTimeToBeParsedOneByOne)
{
    SocketHandler host;
    host.createSocket();
    host.startServer("0.0.0.0", "9999");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::thread thred = std::thread(
        []()
        {
            SocketHandler client;
            client.createSocket();
            client.estabilishConnection("0.0.0.0", "9999");
            client.send("Hurdurdurd");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            client.send("Wruum");
        });
    thred.detach();
    auto client = host.acceptConnection();
    {
        auto [msg, status] = client->receive();
        EXPECT_EQ(status, Status::Correct);
        EXPECT_EQ(msg, "Hurdurdurd");
    }
    {
        auto [msg, status] = client->receive();
        EXPECT_EQ(status, Status::Correct);
        EXPECT_EQ(msg, "Wruum");
    }
}

TEST(SocketHandlerShould, SendAndReceiveStringMessagesWhenReceivedTwoAtOnce)
{
    SocketHandler host;
    host.createSocket();
    host.startServer("0.0.0.0", "9999");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::thread thred = std::thread(
        []()
        {
            SocketHandler client;
            client.createSocket();
            client.estabilishConnection("0.0.0.0", "9999");
            client.send("Hurdurdurd");
            client.send("Wruum");
        });
    thred.detach();
    auto client = host.acceptConnection();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    {
        auto [msg, status] = client->receive();
        EXPECT_EQ(status, Status::Correct);
        EXPECT_EQ(msg, "Hurdurdurd");
    }
    {
        auto [msg, status] = client->receive();
        EXPECT_EQ(status, Status::Correct);
        EXPECT_EQ(msg, "Wruum");
    }
}
}  // namespace unnamed_protocol::test
