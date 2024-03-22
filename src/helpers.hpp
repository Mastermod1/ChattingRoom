#pragma once

#include <ncurses.h>

#include <functional>
#include <memory>

#include "state.hpp"

enum class DisplayState
{
    MainMenu,
    Connect,
    Client,
    Host,
    Exit,
    AfterExit,
};

void getInput(char* buffer, std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& input_window,
              std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& chat_window, int& new_line_index,
              const std::string& name);

class StateFactory
{
  public:
    static std::unique_ptr<State> get(const DisplayState state);
};
