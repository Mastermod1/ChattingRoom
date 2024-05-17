#include "host_state.hpp"

#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>

#include "context.hpp"
#include "helpers.hpp"
#include "logger.hpp"

void HostState::render()
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
        LOG_INFO() << "Socket not created";
        close(socketfd);
        exit(-1);
    }

    struct sockaddr_in addr = {AF_INET, htons(9999), 0};

    int bnd = bind(socketfd, (struct sockaddr*)&addr, sizeof(addr));
    if (bnd == -1)
    {
        LOG_INFO() << "Bind failed";
        close(socketfd);
        exit(-1);
    }

    const std::string& name = form_values.at("name");
    LOG_INFO() << "Extracted name: " << name.c_str();
    std::vector<int> clients;

    listen(socketfd, 10);

    int new_line_index = 0;
    std::thread listener_thread = std::thread(
        [&socketfd, &clients, &name, &chat_window, &new_line_index]
        {
            while (true)
            {
                int clientfd = accept(socketfd, 0, 0);
                clients.push_back(clientfd);
                char name[256] = {0};
                if (recv(clientfd, name, 255, 0) == 0)
                {
                    return 0;
                }
                const std::string welcome_message = std::string(name) + " has connected!";
                std::for_each(clients.begin(), clients.end(),
                              [&clientfd, &welcome_message](const int& client)
                              {
                                  if (client != clientfd)
                                  {
                                      send(client, welcome_message.c_str(), 255, 0);
                                  }
                              });
                mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", welcome_message.c_str());
                new_line_index++;
                box(chat_window.get(), 0, 0);
                wrefresh(chat_window.get());
                LOG_INFO() << "Accepted client: " << std::to_string(clientfd).c_str();

                std::thread printer = std::thread(
                    [&chat_window, &new_line_index, &clients](int clientfd)
                    {
                        while (true)
                        {
                            char buffer[256] = {0};
                            if (recv(clientfd, buffer, 255, 0) == 0)
                            {
                                return 0;
                            }

                            LOG_INFO() << "Send to all from " << std::to_string(clientfd).c_str();
                            std::for_each(clients.begin(), clients.end(),
                                          [&buffer, &clientfd](const int& client)
                                          {
                                              if (client != clientfd)
                                              {
                                                  LOG_INFO() << "Send to client: " << std::to_string(client).c_str();
                                                  send(client, buffer, 255, 0);
                                              }
                                          });
                            mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", buffer);
                            new_line_index++;
                            box(chat_window.get(), 0, 0);
                            wrefresh(chat_window.get());
                        }
                    },
                    clientfd);
                printer.detach();
            }
        });
    listener_thread.detach();

    while (true)
    {
        char buffer[256] = {0};
        getInput(buffer, input_window, chat_window, new_line_index, name);
        std::for_each(clients.begin(), clients.end(), [&buffer](const int& client) { send(client, buffer, 255, 0); });
    }

    close(socketfd);
}

HostState::~HostState() { endwin(); }
