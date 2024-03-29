#pragma once

#include <ncurses.h>

#include <memory>
#include <unordered_map>

#include "state.hpp"

class Context : public std::enable_shared_from_this<Context>
{
    struct Private
    {
    };

  public:
    static std::shared_ptr<Context> create(std::unique_ptr<State> state)
    {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        refresh();

        return std::make_shared<Context>(std::move(state), Private());
    }
    Context(std::unique_ptr<State> state, Private p) : state_(std::move(state)) {}
    ~Context() { endwin(); }

    bool render()
    {
        if (state_ == nullptr)
        {
            return false;
        }
        state_->render();
        return true;
    }
    void changeState(std::unique_ptr<State> state)
    {
        if (state == nullptr)
        {
            state_ = nullptr;
            return;
        }
        state_ = std::move(state);
        state_->setContext(shared_from_this());
    }
    void setContextForState() { state_->setContext(shared_from_this()); }
    std::unordered_map<std::string, std::string> getFormValues() { return form_values_; }
    void setFormValues(const std::unordered_map<std::string, std::string>& form_values) { form_values_ = form_values; }

  private:
    std::unique_ptr<State> state_;
    std::unordered_map<std::string, std::string> form_values_;
};
