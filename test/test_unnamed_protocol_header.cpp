#include <gtest/gtest.h>

#include "src/unnamed_protocol/header.hpp"

namespace unnamed_protocol::header::test
{
TEST(HeaderShould, SetBufferToValuesFromHeader)
{
    Header header{255, 257};
    std::uint8_t buffer[3] = {0};
    setHeader(buffer, header);
    EXPECT_EQ(buffer[0], 255);
    EXPECT_EQ(buffer[1], 257 & 0xff);
    EXPECT_EQ(buffer[2], 257 >> 8);
}

TEST(HeaderShould, GetHeaderWithMessageLength1WhenOnlyLsbIs1)
{
    std::uint8_t buffer[3] = {0b10001000, 1, 0};
    auto header = getHeader(buffer);
    EXPECT_EQ(header.type_, 0b10001000);
    EXPECT_EQ(header.message_length_, 1);
}

TEST(HeaderShould, GetHeaderWithMessageLength256WhenOnlyMsbIs1)
{
    std::uint8_t buffer[3] = {0b10001000, 0, 1};
    auto header = getHeader(buffer);
    EXPECT_EQ(header.type_, 0b10001000);
    EXPECT_EQ(header.message_length_, 256);
}

TEST(HeaderShould, GetHeaderWithMessageLength65535WhenMsbAndLsbIs255)
{
    std::uint8_t buffer[3] = {0b10001000, 255, 255};
    auto header = getHeader(buffer);
    EXPECT_EQ(header.type_, 0b10001000);
    EXPECT_EQ(header.message_length_, 65535);
}
}  // namespace unnamed_protocol::header::test
