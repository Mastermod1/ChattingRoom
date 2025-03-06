#include "src/helpers/helpers.hpp"

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

void getInput(char* buffer, std::unique_ptr<Window>& input_window, const std::string& name)
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
    wmove(*input_window, 1, 1);
    char ch;
    while ((ch = wgetch(*input_window)) != '\n')
    {
        if (ch == 127)
        {
            if (i - name.length() - 2 > 0)
            {
                mvwprintw(*input_window, 1, i - name.length() - 2, " ");
                i--;
                wrefresh(*input_window);
            }
        }
        else
        {
            buffer[i++] = ch;
            mvwaddch(*input_window, 1, i - name.length() - 2, ch);
            wrefresh(*input_window);
        }
        wmove(*input_window, 1, 1 + i - name.length() - 2);
    }
    wclear(*input_window);
    box(*input_window, 0, 0);
    wrefresh(*input_window);
    buffer[i] = '\0';
}
