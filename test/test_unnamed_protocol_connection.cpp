#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "src/unnamed_protocol/socket_handler.hpp"

namespace unnamed_protocol::test
{
TEST(SocketHandlerShould, SendAndReciveStringMessages)
{
    SocketHandler host;
    host.createSocket();
    host.setServerAddress("0.0.0.0", "9999");
    host.listen();
    std::thread thred = std::thread(
        []()
        {
            SocketHandler client;
            client.createSocket();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            client.estabilishConnection("0.0.0.0", "9999");
            client.send("Hurdurdurd");
            client.send("Wruum");
        });
    thred.detach();
    auto client = host.acceptConnection();
    {
        auto [msg, status] = client->recive();
        EXPECT_EQ(status, Status::Correct);
        EXPECT_EQ(msg, "Hurdurdurd");
    }
    {
        auto [msg, status] = client->recive();
        EXPECT_EQ(status, Status::Correct);
        EXPECT_EQ(msg, "Wruum");
    }
}
}  // namespace unnamed_protocol::test
