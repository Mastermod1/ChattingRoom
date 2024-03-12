#include <ncurses.h>

#include <functional>
#include <unordered_map>

#include "connect_menu.hpp"
#include "helpers.hpp"
#include "main_menu.hpp"

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    std::unordered_map<DisplayState, std::function<DisplayState()>> state_map = {
        {DisplayState::MainMenu, renderMainMenu}, {DisplayState::Connect, renderConnectMenu}};

    DisplayState state = DisplayState::MainMenu;
    while (state != DisplayState::Exit)
    {
        state = state_map.at(state)();
    }

    endwin();
}
