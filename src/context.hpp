#pragma once

#include <memory>
#include <unordered_map>

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
    std::unordered_map<std::string, std::string> getFormValues() { return form_values_; }
    void setFormValues(const std::unordered_map<std::string, std::string>& form_values) { form_values_ = form_values; }

  private:
    std::unique_ptr<State> state_;
    std::unordered_map<std::string, std::string> form_values_;
};
