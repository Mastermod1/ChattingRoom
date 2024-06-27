#pragma once

#include <memory>
#include <thread>

#include "state.hpp"
#include "tcp_ip_connection.hpp"

class Context;

class ClientState : public State
{
  public:
    ClientState() = default;
    ClientState(const ClientState& rhs) = default;
    ClientState(ClientState&& rhs) noexcept = default;
    ClientState& operator=(const ClientState& rhs) = default;
    ClientState& operator=(ClientState&& rhs) noexcept = default;
    virtual ~ClientState();

    void render() override;
    void setContext(const std::shared_ptr<Context>& ctx) override { ctx_ = ctx; }

  private:
    std::weak_ptr<Context> ctx_;
    std::unique_ptr<std::thread> receiver_thread_;
    TcpIpConnection connection_;
};
