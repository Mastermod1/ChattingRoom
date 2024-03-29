#pragma once

#include <memory>

#include "state.hpp"

class Context;

class ClientState : public State
{
  public:
    ClientState() = default;
    ClientState(const ClientState& rhs) = default;
    ClientState(ClientState&& rhs) = default;
    ClientState& operator=(const ClientState& rhs) = default;
    ClientState& operator=( ClientState&& rhs) = default;

    void render() const override;
    void setContext(const std::shared_ptr<Context>& ctx) override { ctx_ = ctx; }

  private:
    std::weak_ptr<Context> ctx_;
};
