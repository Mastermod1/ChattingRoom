#pragma once

#include <cstdint>

namespace unnamed_protocol::header
{
struct Header
{
    std::uint8_t type_;
    std::uint16_t message_length_;
};

void setHeader(std::uint8_t buffer[], const Header& header);
Header getHeader(std::uint8_t buffer[]);
}  // namespace hesitant::header
