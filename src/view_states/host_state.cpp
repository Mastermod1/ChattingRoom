#include "src/view_states/host_state.hpp"

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

#include "src/helpers/helpers.hpp"
#include "src/logger/logger.hpp"
#include "src/view_states/context.hpp"

using namespace unnamed_protocol;

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

    if (connection_.createSocket() == Status::Error)
    {
        LOG_INFO() << "Socket not created";
        exit(-1);
    }

    const std::string& address = form_values.at("address");
    const std::string& port = form_values.at("port");

    if (connection_.startServer(address, port) == Status::Error)
    {
        LOG_INFO() << "Bind failed";
        exit(-1);
    }

    const std::string& name = form_values.at("name");
    LOG_INFO() << "Extracted name: " << name.c_str();
    std::vector<std::shared_ptr<SocketHandler>> clients;

    int new_line_index = 0;
    std::thread listener_thread = std::thread(
        [this, &clients, &chat_window, &new_line_index]
        {
            while (true)
            {
                auto client_connection = connection_.acceptConnection();
                clients.push_back(client_connection);
                auto [name, status] = client_connection->receive();
                if (status == Status::Error)
                {
                    return 0;
                }

                const std::string welcome_message = name + " has connected!";
                std::for_each(clients.begin(), clients.end(),
                              [&client_connection, &welcome_message](const auto& client)
                              {
                                  if (*client != *client_connection)
                                  {
                                      client->send(welcome_message);
                                  }
                              });
                mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", welcome_message.c_str());
                new_line_index++;
                box(chat_window.get(), 0, 0);
                wrefresh(chat_window.get());
                // LOG_INFO() << "Accepted client: " << std::to_string(client_connection.socket_).c_str();

                std::thread printer = std::thread(
                    [&chat_window, &new_line_index, &clients](std::shared_ptr<SocketHandler> client_connection)
                    {
                        while (true)
                        {
                            auto [message, status] = client_connection->receive();
                            if (status == Status::Error)
                            {
                                mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", "Error");
                                new_line_index++;
                                box(chat_window.get(), 0, 0);
                                wrefresh(chat_window.get());
                                return 0;
                            }

                            // LOG_INFO() << "Send to all from " << std::to_string(clientfd).c_str();
                            std::string msg = message;
                            std::for_each(clients.begin(), clients.end(),
                                          [&msg, &client_connection](const auto& client)
                                          {
                                              if (*client != *client_connection)
                                              {
                                                  // LOG_INFO() << "Send to client: " << std::to_string(client).c_str();
                                                  client->send(msg);
                                              }
                                          });
                            mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", message.c_str());
                            new_line_index++;
                            box(chat_window.get(), 0, 0);
                            wrefresh(chat_window.get());
                        }
                    },
                    client_connection);
                printer.detach();
            }
        });
    listener_thread.detach();

    while (true)
    {
        char buffer[256] = {0};
        getInput(buffer, input_window, chat_window, new_line_index, name);
        std::for_each(clients.begin(), clients.end(), [&buffer](const auto& client) { client->send(buffer); });
    }
}

HostState::~HostState() { endwin(); }
