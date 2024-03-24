#pragma once

#include <memory>

#include "state.hpp"

class Context;

class ClientState : public State
{
  public:
    void render() const override;
    void setContext(const std::shared_ptr<Context>& ctx) override { ctx_ = ctx; }

    virtual ~ClientState() = default;

  private:
    std::weak_ptr<Context> ctx_;
};
