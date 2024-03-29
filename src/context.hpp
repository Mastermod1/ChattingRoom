#pragma once

#include <memory>
#include <unordered_map>

#include "state.hpp"

class Context : public std::enable_shared_from_this<Context>
{
    struct Private
    {
    };

  public:
    static std::shared_ptr<Context> create(std::unique_ptr<State> state);
    ~Context();
    Context(std::unique_ptr<State> state, Private p) : state_(std::move(state)) {}

    bool render();
    void changeState(std::unique_ptr<State> state);
    void setContextForState();
    std::unordered_map<std::string, std::string> getFormValues();
    void setFormValues(const std::unordered_map<std::string, std::string>& form_values);

  private:
    std::unique_ptr<State> state_;
    std::unordered_map<std::string, std::string> form_values_;
};
