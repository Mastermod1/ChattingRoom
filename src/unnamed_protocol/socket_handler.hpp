#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <memory>

#include "src/unnamed_protocol/header.hpp"
#include "src/common/status_enum.hpp"
#include "src/helpers.hpp"

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
        const struct sockaddr_in addr = {AF_INET, htons(std::stoi(port)), dst_ip_addr};
        if (connect(socket_, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        {
            close(socket_);
            return Status::Error;
        }
        return Status::Correct;
    }

    Status setServerAddress(const std::string& address, const std::string& port)
    {
        const in_addr_t server_ip = convertStringAddressToUint32(address);
        struct sockaddr_in addr = {AF_INET, htons(std::stoi(port)), server_ip};
        if (bind(socket_, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        {
            return Status::Error;
        }
        return Status::Correct;
    }

    void listen() { ::listen(socket_, 10); }

    std::pair<std::string, Status> recive()
    {
        std::uint8_t buffer[MAX_BUFFER_SIZE] = {0};
        // TODO handle situation when recv is longer/shorter than message length
        int recived_size = recv(socket_, buffer, MAX_BUFFER_SIZE, 0);
        const auto header = unnamed_protocol::header::getHeader(buffer);
        std::string string_payload = reinterpret_cast<const char*>(buffer + 3);
        return {string_payload, Status::Correct};
    }

    std::shared_ptr<SocketHandler> acceptConnection()
    {
        int connection = accept(socket_, 0, 0);
        return std::make_shared<SocketHandler>(connection);
    }

    Status send(const std::string& message) const
    {
        std::uint8_t buffer[MAX_BUFFER_SIZE] = {0};
        std::uint16_t message_size = message.size();
        unnamed_protocol::header::setHeader(buffer, unnamed_protocol::header::Header{0, message_size});
        memcpy(buffer + 3, message.c_str(), message_size);
        if (::send(socket_, buffer, 3 + message_size, 0) == -1)
        {
            return Status::Error;
        }
        return Status::Correct;
    }

    ~SocketHandler() { close(socket_); }

  private:
    int socket_;
};
}  // namespace unnamed_protocol
