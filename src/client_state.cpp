#include "client_state.hpp"

#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <unordered_map>

#include "context.hpp"
#include "helpers.hpp"

void ClientState::render() const
{
    std::unique_ptr<WINDOW, std::function<void(WINDOW*)>> chat_window(newwin(LINES - 4, COLS, 0, 0), delwin);
    std::unique_ptr<WINDOW, std::function<void(WINDOW*)>> input_window(newwin(4, COLS, LINES - 4, 0), delwin);

    box(chat_window.get(), 0, 0);
    box(input_window.get(), 0, 0);
    wrefresh(chat_window.get());
    wrefresh(input_window.get());

    std::unordered_map<std::string, std::string> form_values;
    if (auto ptr = ctx_.lock())
    {
        form_values = ptr->getFormValues();
    }

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        std::cerr << "Socket not created\n";
        close(socketfd);
        exit(-1);
    }

    struct sockaddr_in addr = {AF_INET, htons(9999), 0};

    if (connect(socketfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cerr << "Connection failed\n";
        close(socketfd);
        exit(-1);
    }

    int new_line_index = 0;
    std::thread printer = std::thread(
        [&socketfd, &chat_window, &new_line_index]()
        {
            while (true)
            {
                char buffer[256] = {0};
                if (recv(socketfd, buffer, 255, 0) == 0)
                {
                    return 0;
                }
                mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s: %s", "other", buffer);
                new_line_index++;
                box(chat_window.get(), 0, 0);
                wrefresh(chat_window.get());
            }
        });
    printer.detach();

    while (true)
    {
        char buffer[256] = {0};
        getInput(buffer, input_window, chat_window, new_line_index, form_values.at("name"));
        send(socketfd, buffer, 255, 0);
    }

    close(socketfd);
    endwin();
}
