#include "helpers.hpp"

#include <iostream>

in_addr_t convertStringAddressToUint32(const std::string& address)
{
    in_addr_t ip_address = 0;
    std::uint8_t offset = 0;
    std::size_t start_pos = 0;
    for (int i = 0; i < 4; ++i)
    {
        auto dot_pos = address.find('.', start_pos);
        ip_address += std::stoi(address.substr(start_pos, dot_pos - start_pos)) << offset;
        offset += 8;
        start_pos = dot_pos + 1;
    }
    return ip_address;
}

void getInput(char* buffer, std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& input_window,
              std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& chat_window, int& new_line_index,
              const std::string& name)
{
    int i = 0;
    for (const auto& c : name)
    {
        buffer[i] = c;
        i++;
    }
    buffer[i] = ':';
    i++;
    buffer[i] = ' ';
    i++;
    wmove(input_window.get(), 1, 1);
    char ch;
    while ((ch = wgetch(input_window.get())) != '\n')
    {
        if (ch == 127)
        {
            if (i - name.length() - 2 > 0)
            {
                mvwprintw(input_window.get(), 1, i - name.length() - 2, " ");
                i--;
                wrefresh(input_window.get());
            }
        }
        else
        {
            buffer[i++] = ch;
            mvwaddch(input_window.get(), 1, i - name.length() - 2, ch);
            wrefresh(input_window.get());
        }
        wmove(input_window.get(), 1, 1 + i - name.length() - 2);
    }
    wclear(input_window.get());
    box(input_window.get(), 0, 0);
    wrefresh(input_window.get());
    buffer[i] = '\0';
    mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", buffer);
    new_line_index++;
    box(chat_window.get(), 0, 0);
    wrefresh(chat_window.get());
}
