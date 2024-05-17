#pragma once

#include <memory>

#include "state.hpp"

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
};
