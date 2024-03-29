#pragma once

#include <memory>

#include "state.hpp"

class Context;

class ConnectMenuState : public State
{
  public:
    ConnectMenuState() = default;
    ConnectMenuState(const ConnectMenuState& rhs) = default;
    ConnectMenuState(ConnectMenuState&& rhs) = default;
    ConnectMenuState& operator=(const ConnectMenuState& rhs) = default;
    ConnectMenuState& operator=( ConnectMenuState&& rhs) = default;
    virtual ~ConnectMenuState() = default;

    void render() override;
    void setContext(const std::shared_ptr<Context>& ctx) override { ctx_ = ctx; }

  private:
    std::weak_ptr<Context> ctx_;
};
