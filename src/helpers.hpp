#pragma once

#include <ncurses.h>

#include <functional>
#include <memory>

enum class DisplayState
{
    MainMenu,
    Connect,
    Host,
    Exit,
    AfterExit,
};

void getInput(char* buffer, std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& input_window,
              std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& chat_window, int& new_line_index);

