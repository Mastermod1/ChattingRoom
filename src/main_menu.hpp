#pragma once

#include <ncurses.h>

#include <functional>
#include <memory>
#include <string>

#include "helpers.hpp"
#include "menu_wrapper.hpp"

DisplayState renderMainMenu()
{
    int y_size = 15;
    int x_size = 30;
    std::unique_ptr<WINDOW, std::function<void(WINDOW*)>> menu_window(
        newwin(y_size, x_size, (LINES - y_size) / 2, (COLS - x_size) / 2), delwin);
    box(menu_window.get(), 0, 0);
    wrefresh(menu_window.get());

    MenuWrapper menu({"Connect", "Host", "Exit"});
    set_menu_win(menu, menu_window.get());
    set_menu_sub(menu, derwin(menu_window.get(), y_size - 2, x_size - 2, 1, 1));
    post_menu(menu);
    refresh();
    wrefresh(menu_window.get());

    int ch;
    while ((ch = getch()) != KEY_F(1))
    {
        switch (ch)
        {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10:  // Enter
            {
                ITEM* cur = current_item(menu);
                std::string name = cur->name.str;
                if (name == "Connect")
                {
                    mvprintw(0, 0, "Connect");
                }
                else if (name == "Host")
                {
                    mvprintw(0, 0, "Host");
                }
                else if (name == "Exit")
                {
                    mvprintw(0, 0, "Exit");
                    return DisplayState::Exit;
                }
                break;
            }
        }
        wrefresh(stdscr);
        wrefresh(menu_window.get());
    }
}
