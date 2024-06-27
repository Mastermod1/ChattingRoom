#include "src/view_states/context.hpp"

#include <ncurses.h>

std::shared_ptr<Context> Context::create(std::unique_ptr<State> state)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    return std::make_shared<Context>(std::move(state), Private());
}

Context::~Context() { endwin(); }

bool Context::render()
{
    if (state_ == nullptr)
    {
        return false;
    }
    state_->render();
    return true;
}

void Context::changeState(std::unique_ptr<State> state)
{
    if (state == nullptr)
    {
        state_ = nullptr;
        return;
    }
    state_ = std::move(state);
    state_->setContext(shared_from_this());
}

void Context::setContextForState() { state_->setContext(shared_from_this()); }

std::unordered_map<std::string, std::string> Context::getFormValues() { return form_values_; }

void Context::setFormValues(const std::unordered_map<std::string, std::string>& form_values)
{
    form_values_ = form_values;
}
