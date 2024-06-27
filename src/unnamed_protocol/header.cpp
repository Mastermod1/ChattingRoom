#include "src/unnamed_protocol/header.hpp"

namespace unnamed_protocol::header
{
void setHeader(std::uint8_t buffer[], const Header& header)
{
    buffer[0] = header.type_;
    buffer[1] = static_cast<std::uint8_t>(header.message_length_);
    buffer[2] = static_cast<std::uint8_t>(header.message_length_ >> 8);
}

Header getHeader(std::uint8_t buffer[])
{
    Header header;
    header.type_ = buffer[0];
    header.message_length_ = buffer[1] + (buffer[2] << 8);
    return header;
}
}  // namespace unnamed_protocol::header
