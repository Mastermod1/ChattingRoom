#pragma once

#include <vector>
#include <cstdint>

class TcpHeader
{
  public:
    TcpHeader(const std::vector<std::uint8_t> buffer) : buffer_(buffer) {}
    std::uint16_t getMessageSize() { return *reinterpret_cast<std::uint16_t*>(buffer_.data() + 1); }

  private:
    std::vector<std::uint8_t> buffer_;
};

class TcpHeaderBuilder
{
  public:
    std::vector<std::uint8_t> build(const std::uint16_t message_size)
    {
        buffer_[0] = 0b00000000;
        for (int i = 0; i < 2; i++)
        {
            buffer_[i + 1] = static_cast<std::uint8_t>(message_size >> (i * 8));
        }
        return buffer_;
    }
  private:
    std::vector<std::uint8_t> buffer_ = std::vector<std::uint8_t>(3, 0);
};
