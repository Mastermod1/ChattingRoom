#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "helpers.hpp"
#include "tcp_header.hpp"

enum class Status
{
    Correct,
    Error
};

struct Message
{
    std::uint8_t header{0};
    std::uint16_t message_length{0};
    std::uint8_t data[];
};

class TcpIpConnection
{
  public:
    TcpIpConnection() = default;
    TcpIpConnection(const TcpIpConnection& rhs) = default;
    TcpIpConnection(TcpIpConnection&& rhs) noexcept = default;
    TcpIpConnection& operator=(const TcpIpConnection& rhs) noexcept = default;
    TcpIpConnection& operator=(TcpIpConnection&& rhs) = default;

    TcpIpConnection(const int connection) : socket_(connection) {}

    bool operator==(const TcpIpConnection& rhs) const { return socket_ == rhs.socket_; }
    bool operator!=(const TcpIpConnection& rhs) const { return socket_ != rhs.socket_; }

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
        void* mem = std::malloc(2048);
        Message* msg = new (mem) Message;
        int rec = recv(socket_, msg, 2048, 0);
        std::string str = "";
        auto status = rec > 0 ? Status::Correct : Status::Error;
        for (int i = 0; i < msg->message_length; i++)
        {
            str += msg->data[i];
        }
        free(msg);
        return {str, status};
    }

    std::shared_ptr<TcpIpConnection> acceptConnection()
    {
        int connection = accept(socket_, 0, 0);
        return std::make_shared<TcpIpConnection>(connection);
    }

    Status send(const std::string& message) const
    {
        void* mem = std::malloc(2048);
        Message* msg = new (mem) Message;
        msg->message_length = message.size();
        for (int i = 0; i < message.length(); ++i)
        {
            msg->data[i] = message[i];
        }
        if (::send(socket_, msg, sizeof(Message) + message.length(), 0) == -1)
        {
            free(msg);
            return Status::Error;
        }
        free(msg);
        return Status::Correct;
    }

    ~TcpIpConnection() { close(socket_); }

  private:
    int socket_;
};
