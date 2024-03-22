#pragma once

#include <memory>

#include "state.hpp"

class Context : public std::enable_shared_from_this<Context>
{
  public:
    Context(std::unique_ptr<State> state) : state_(std::move(state)) {}
    void render() { state_->render(); }
    void changeState(std::unique_ptr<State> state)
    {
        state_ = std::move(state);
        state_->setContext(shared_from_this());
        render();
    }
    void setContextForState() { state_->setContext(shared_from_this()); }

  private:
    std::unique_ptr<State> state_;
};
