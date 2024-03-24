#include "connect_menu_state.hpp"

#include <form.h>
#include <ncurses.h>

#include <cctype>
#include <functional>
#include <memory>
#include <string>

#include "context.hpp"
#include "form_builder.hpp"
#include "form_wrapper.hpp"
#include "helpers.hpp"

void ConnectMenuState::render() const
{
    int y_size = 15;
    int x_size = 30;
    std::unique_ptr<WINDOW, std::function<void(WINDOW*)>> menu_window(
        newwin(y_size, x_size, (LINES - y_size) / 2, (COLS - x_size) / 2), delwin);
    box(menu_window.get(), 0, 0);
    wrefresh(menu_window.get());

    FormWrapper form = FormBuilder(menu_window.get())
                           .addLabelField("Name:")
                           .addInputField()
                           .addLabelField("Address:")
                           .addInputField()
                           .addLabelField("Port:")
                           .addInputField()
                           .addLabelField("Connect")
                           .addLabelField("Back")
                           .getForm();

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
                    if (auto ptr = ctx_.lock())
                    {
                        ptr->changeState(StateFactory::get(DisplayState::MainMenu));
                    }
                    return;
                }
                else if (value == "Connect")
                {
                    const auto& form_values = form.submitFormValues();
                    int i = 1;
                    for (auto x : form_values)
                    {
                        mvprintw(i++, 0, "%s.", (x.first + " " + x.second).c_str());
                    }
                    if (auto ptr = ctx_.lock())
                    {
                        ptr->setFormValues(form_values);
                        ptr->changeState(StateFactory::get(DisplayState::Client));
                    }
                    return;
                }
                break;
            }
            case KEY_BACKSPACE:
            {
                form_driver(form, REQ_DEL_PREV);
            }
            default:
            {
                if (isprint(ch))
                {
                    form_driver(form, ch);
                }
                break;
            }
        }
        wrefresh(menu_window.get());
    }
}
