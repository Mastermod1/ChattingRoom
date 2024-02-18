#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>

int main()
{
    initscr();
    refresh();
    char ch;
    WINDOW* chat_window = newwin(40, 120, 0, 0);
    WINDOW* input_window = newwin(5, 120, 40, 0);

    box(chat_window, 0, 0);
    box(input_window, 0, 0);
    wrefresh(chat_window);
    wrefresh(input_window);
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        std::cerr << "Socket not created\n";
        close(socketfd);
        exit(-1);
    }

    struct sockaddr_in addr = {AF_INET, htons(9999), 0};

    connect(socketfd, (struct sockaddr*)&addr, sizeof(addr));
    std::thread printer = std::thread(
        [&socketfd, &chat_window]()
        {
            int counter = 0;
            while (true)
            {
                char buffer[256] = {0};
                if (recv(socketfd, buffer, 255, 0) == 0)
                {
                    return 0;
                }
                mvwprintw(chat_window, 1 + counter, 1, "%s", buffer);
                wrefresh(chat_window);
                counter++;
            }
        });
    printer.detach();

    while (true)
    {
        char buffer[256] = {0};
        read(0, buffer, 255);
        send(socketfd, buffer, 255, 0);
    }
    close(socketfd);
    delwin(chat_window);
    delwin(input_window);
    endwin();
}
