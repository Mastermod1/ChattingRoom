#pragma once

#include <form.h>
#include <ncurses.h>

#include <functional>
#include <memory>
#include <string>

#include "form_wrapper.hpp"
#include "helpers.hpp"

std::string truncate(const std::string& str)
{
    int last_letter_index = str.size() - 1;
    while (str[last_letter_index] == ' ' and last_letter_index >= 0) last_letter_index--;
    return str.substr(0, last_letter_index + 1);
}

DisplayState renderConnectMenu()
{
    int y_size = 15;
    int x_size = 30;
    std::unique_ptr<WINDOW, std::function<void(WINDOW*)>> menu_window(
        newwin(y_size, x_size, (LINES - y_size) / 2, (COLS - x_size) / 2), delwin);
    box(menu_window.get(), 0, 0);
    wrefresh(menu_window.get());

    FormWrapper form({"Address:", "Port:", "Back"}, menu_window.get());
    set_form_win(form, menu_window.get());
    set_form_sub(form, derwin(menu_window.get(), y_size - 2, x_size - 2, 1, 1));
    post_form(form);
    refresh();
    wrefresh(menu_window.get());

    int ch;
    while ((ch = getch()) != KEY_F(1))
    {
        switch (ch)
        {
            case KEY_DOWN:
            {
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            }
            case KEY_UP:
            {
                form_driver(form, REQ_PREV_FIELD);
                form_driver(form, REQ_END_LINE);
                break;
            }
            case 10:  // Enter
            {
                FIELD* cur = current_field(form);
                std::string value = truncate(field_buffer(cur, 0));
                mvprintw(0, 0, "%s.", value.c_str());
                if (value == "Back")
                {
                    return DisplayState::MainMenu;
                }
                break;
            }
            default:
            {
                form_driver(form, ch);
                break;
            }
        }
        wrefresh(menu_window.get());
    }
    return DisplayState::Exit;
}
