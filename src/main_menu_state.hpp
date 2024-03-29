#pragma once

#include <memory>

#include "state.hpp"

class Context;

class MainMenuState : public State
{
  public:
    MainMenuState() = default;
    MainMenuState(const MainMenuState& rhs) = default;
    MainMenuState(MainMenuState&& rhs) = default;
    MainMenuState& operator=(const MainMenuState& rhs) = default;
    MainMenuState& operator=( MainMenuState&& rhs) = default;
    virtual ~MainMenuState() = default;

    void render() const override;
    void setContext(const std::shared_ptr<Context>& ctx) override { ctx_ = ctx; }

  private:
    std::weak_ptr<Context> ctx_;
};
