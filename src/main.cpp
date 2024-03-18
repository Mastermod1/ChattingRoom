#include <ncurses.h>

#include <functional>
#include <unordered_map>

#include "client_view.hpp"
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

    std::unordered_map<std::string, std::string> form;

    std::unordered_map<DisplayState, std::function<DisplayState()>> state_map = {
        {DisplayState::MainMenu, renderMainMenu},
        {DisplayState::Connect, [&form]() { return renderConnectMenu(form); }},
        {DisplayState::Client, [&form]() { return renderClientView(form); }}};

    DisplayState state = DisplayState::MainMenu;
    while (state != DisplayState::Exit)
    {
        state = state_map.at(state)();
    }

    endwin();
}
