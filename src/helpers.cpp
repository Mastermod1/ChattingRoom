#include "helpers.hpp"

#include <ncurses.h>

#include <memory>

#include "client_state.hpp"
#include "connect_menu_state.hpp"
#include "main_menu_state.hpp"

void getInput(char* buffer, std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& input_window,
              std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& chat_window, int& new_line_index,
              const std::string& name)
{
    char ch;
    int i = 0;
    wmove(input_window.get(), 1, 1 + i);
    while ((ch = wgetch(input_window.get())) != '\n')
    {
        if (ch == KEY_BACKSPACE || ch == 127)
        {
            if (i > 0)
            {
                mvwprintw(input_window.get(), 1, 1 + i, " ");
                i--;
                wrefresh(input_window.get());
            }
        }
        else
        {
            buffer[i++] = ch;
            mvwaddch(input_window.get(), 1, 1 + i, ch);
            wrefresh(input_window.get());
        }
        wmove(input_window.get(), 1, 2 + i);
    }
    wclear(input_window.get());
    box(input_window.get(), 0, 0);
    wrefresh(input_window.get());
    buffer[i] = '\0';
    mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s: %s", name.c_str(), buffer);
    new_line_index++;
    box(chat_window.get(), 0, 0);
    wrefresh(chat_window.get());
}

std::unique_ptr<State> StateFactory::get(const DisplayState state)
{
    switch (state)
    {
        case DisplayState::Connect:
            return std::make_unique<ConnectMenuState>();
        case DisplayState::Client:
            return std::make_unique<ClientState>();
        default:
        case DisplayState::MainMenu:
            return std::make_unique<MainMenuState>();
    }

    return std::make_unique<MainMenuState>();
}
