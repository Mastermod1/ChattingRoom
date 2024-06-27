#pragma once

#include <memory>

#include "src/view_states/state.hpp"

class Context;

class HostMenuState : public State
{
  public:
    HostMenuState() = default;
    HostMenuState(const HostMenuState& rhs) = default;
    HostMenuState(HostMenuState&& rhs) = default;
    HostMenuState& operator=(const HostMenuState& rhs) = default;
    HostMenuState& operator=( HostMenuState&& rhs) = default;
    virtual ~HostMenuState() = default;

    void render() override;
    void setContext(const std::shared_ptr<Context>& ctx) override { ctx_ = ctx; }

  private:
    std::weak_ptr<Context> ctx_;
};
