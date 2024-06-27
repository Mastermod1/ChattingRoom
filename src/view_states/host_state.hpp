#pragma once

#include <memory>

#include "src/view_states/state.hpp"
#include "src/unnamed_protocol/socket_handler.hpp"

class Context;

class HostState : public State
{
  public:
    HostState() = default;
    HostState(const HostState& rhs) = default;
    HostState(HostState&& rhs) = default;
    HostState& operator=(const HostState& rhs) = default;
    HostState& operator=(HostState&& rhs) = default;
    virtual ~HostState();

    void render() override;
    void setContext(const std::shared_ptr<Context>& ctx) override { ctx_ = ctx; }

  private:
    std::weak_ptr<Context> ctx_;
    unnamed_protocol::SocketHandler connection_; 
};
