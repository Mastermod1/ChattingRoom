#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <functional>
#include <memory>

#include <iostream>
#include <thread>

int main()
{
    initscr();
    cbreak();
    noecho();
    refresh();

    std::unique_ptr<WINDOW, std::function<void(WINDOW*)>> chat_window(newwin(LINES - 4, COLS, 0, 0), delwin);
    std::unique_ptr<WINDOW, std::function<void(WINDOW*)>> input_window(newwin(4, COLS, LINES - 4, 0), delwin);
    box(chat_window.get(), 0, 0);
    box(input_window.get(), 0, 0);
    wrefresh(chat_window.get());
    wrefresh(input_window.get());

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        std::cerr << "Socket not created\n";
        close(socketfd);
        exit(-1);
    }

    struct sockaddr_in addr = {AF_INET, htons(9999), 0};

    int bnd = bind(socketfd, (struct sockaddr *)&addr, sizeof(addr));
    if (bnd == -1)
    {
        std::cerr << "Bind failed\n";
        close(socketfd);
        exit(-1);
    }

    int lsit = listen(socketfd, 10);

    int clientfd = accept(socketfd, 0, 0);

    int new_line_index = 0;
    std::thread printer = std::thread(
        [&clientfd, &chat_window, &new_line_index]()
        {
            while (true)
            {
                char buffer[256] = {0};
                if (recv(clientfd, buffer, 255, 0) == 0)
                {
                    return 0;
                }
                mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", buffer);
                new_line_index++;
                box(chat_window.get(), 0, 0);
                wrefresh(chat_window.get());
            }
            
        });
    printer.detach();

    while (true)
    {
        char buffer[256] = {0}, ch;
        int i = 0;
        while ((ch = wgetch(input_window.get())) != '\n')
        {
            if (ch == KEY_BACKSPACE || ch == 127)
            {
                if (i > 0)
                {
                    i--;
                    mvwprintw(input_window.get(), 1, 1 + i, " ");
                    wrefresh(input_window.get());
                }
            }
            else
            {
                buffer[i++] = ch;
                mvwaddch(input_window.get(), 1, 1 + i, ch);
                wrefresh(input_window.get());
            }
        }
        wclear(input_window.get());
        box(input_window.get(), 0, 0);
        wrefresh(input_window.get());
        buffer[i] = '\0';
        mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", buffer);
        new_line_index++;
        box(chat_window.get(), 0, 0);
        wrefresh(chat_window.get());
        send(clientfd, buffer, 255, 0);
    }

    close(socketfd);
    endwin();
}
