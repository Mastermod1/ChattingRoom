#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "../src/tcp_header.hpp"
#include "../src/tcp_ip_connection.hpp"

TEST(TcpHeaderShoudl, ReturnCorrectMessageSize)
{
    TcpHeader header{{0, 2, 1}};
    EXPECT_EQ(header.getMessageSize(), 2 + (1 << 8));
}

TEST(TcpHeaderBuilderShould, Return)
{
    TcpHeaderBuilder builder;
    {
        auto header = builder.build(257);
        EXPECT_EQ(header[0], 0);
        EXPECT_EQ(header[1], 1);
        EXPECT_EQ(header[2], 1);
        TcpHeader hdr(header);
        EXPECT_EQ(hdr.getMessageSize(), 257);
    }
    {
        auto header = builder.build(65535);
        EXPECT_EQ(header[0], 0);
        EXPECT_EQ(header[1], 255);
        EXPECT_EQ(header[2], 255);
        TcpHeader hdr(header);
        EXPECT_EQ(hdr.getMessageSize(), 65535);
    }
}

TEST(TcpIpConnectionShould, Connect)
{
    TcpIpConnection host;
    host.createSocket();
    host.setServerAddress("0.0.0.0", "9999");
    host.listen();
    std::thread thred = std::thread(
        []()
        {
            TcpIpConnection client;
            client.createSocket();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            client.estabilishConnection("0.0.0.0", "9999");
            client.send("Hurdurdurd");
            client.send("Wruum");
        });
    thred.detach();
    auto client = host.acceptConnection();
    {
        auto [msg, status] = client.recive();
        EXPECT_EQ(status, Status::Correct);
        EXPECT_EQ(msg, "Hurdurdurd");
    }
    {
        auto [msg, status] = client.recive();
        EXPECT_EQ(status, Status::Correct);
        EXPECT_EQ(msg, "Wruum");
    }
}
