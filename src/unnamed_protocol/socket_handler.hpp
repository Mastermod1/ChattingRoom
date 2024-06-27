#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <memory>

#include "src/common/status_enum.hpp"
#include "src/helpers/helpers.hpp"
#include "src/unnamed_protocol/header.hpp"

constexpr int MAX_BUFFER_SIZE = 2048;

namespace unnamed_protocol
{
class SocketHandler
{
  public:
    SocketHandler() = default;
    SocketHandler(const SocketHandler& rhs) = default;
    SocketHandler(SocketHandler&& rhs) noexcept = default;
    SocketHandler& operator=(const SocketHandler& rhs) noexcept = default;
    SocketHandler& operator=(SocketHandler&& rhs) = default;

    SocketHandler(const int connection) : socket_(connection) {}

    bool operator==(const SocketHandler& rhs) const { return socket_ == rhs.socket_; }
    bool operator!=(const SocketHandler& rhs) const { return socket_ != rhs.socket_; }

    Status createSocket()
    {
        socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_ == -1)
        {
            close(socket_);
            return Status::Error;
        }
        return Status::Correct;
    }

    Status estabilishConnection(const std::string& address, const std::string& port)
    {
        const in_addr_t dst_ip_addr = convertStringAddressToUint32(address);
        const struct sockaddr_in addr = {AF_INET, htons(std::stoi(port)), {dst_ip_addr}};
        if (connect(socket_, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        {
            close(socket_);
            return Status::Error;
        }
        return Status::Correct;
    }

    Status startServer(const std::string& address, const std::string& port)
    {
        const in_addr_t server_ip = convertStringAddressToUint32(address);
        struct sockaddr_in addr = {AF_INET, htons(std::stoi(port)), {server_ip}};
        if (bind(socket_, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        {
            return Status::Error;
        }
        listen(socket_, 10);
        return Status::Correct;
    }

    std::pair<std::string, Status> receive()
    {
        std::uint8_t buffer[2 * MAX_BUFFER_SIZE] = {0};
        std::memcpy(buffer, cached_buffer_, cached_buffer_size_);
        const auto received_size = recv(socket_, buffer + cached_buffer_size_, MAX_BUFFER_SIZE, 0);
        const auto header = unnamed_protocol::header::getHeader(buffer);
        const std::string string_payload = reinterpret_cast<const char*>(buffer + header.GetHeaderSize());
        if (received_size > header.GetFrameSize() or cached_buffer_size_ != 0)
        {
            cached_buffer_size_ = std::max(0LL, received_size - header.GetFrameSize());
            std::memcpy(cached_buffer_, buffer + header.GetFrameSize(), cached_buffer_size_);
        }
        return {string_payload, string_payload.length() != 0 ? Status::Correct : Status::Error};
    }

    std::shared_ptr<SocketHandler> acceptConnection()
    {
        int connection = accept(socket_, 0, 0);
        return std::make_shared<SocketHandler>(connection);
    }

    Status send(const std::string& message) const
    {
        std::uint8_t buffer[MAX_BUFFER_SIZE] = {0};
        const std::uint16_t message_size = message.size();
        const unnamed_protocol::header::Header header{0, message_size};
        unnamed_protocol::header::setHeader(buffer, header);
        std::memcpy(buffer + header.GetHeaderSize(), message.c_str(), message_size);
        if (::send(socket_, buffer, header.GetFrameSize(), 0) == -1)
        {
            return Status::Error;
        }
        return Status::Correct;
    }

    ~SocketHandler() { close(socket_); }

  private:
    int socket_;
    std::uint8_t cached_buffer_[MAX_BUFFER_SIZE] = {0};
    long long cached_buffer_size_ = 0;
};
}  // namespace unnamed_protocol
