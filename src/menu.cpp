#include <menu.h>
#include <ncurses.h>

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "helpers.hpp"

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    int y_size = 15;
    int x_size = 30;
    std::unique_ptr<WINDOW, std::function<void(WINDOW*)>> chat_window(
        newwin(y_size, x_size, (LINES - y_size) / 2, (COLS - x_size) / 2), delwin);
    box(chat_window.get(), 0, 0);
    wrefresh(chat_window.get());

    char* arr[] = {"Connect", "Host", "Exit"};
    ITEM** items = (ITEM**)malloc(4 * sizeof(ITEM*));
    for (int i = 0; i < 3; ++i)
    {
        items[i] = new_item("D", arr[i]);
    }
    items[3] = (ITEM*)NULL;

    MENU* menu = new_menu(items);
    set_menu_win(menu, chat_window.get());
    set_menu_sub(menu, derwin(chat_window.get(), y_size - 2, x_size - 2, 1, 1));
    refresh();
    post_menu(menu);
    wrefresh(chat_window.get());

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
                break;
            }
        }
        wrefresh(chat_window.get());
    }

    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < 4; i++) free_item(items[i]);
    endwin();
}
