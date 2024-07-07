#include "src/view_states/client_state.hpp"

#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>

#include "src/view_states/context.hpp"
#include "src/helpers/helpers.hpp"
#include "src/logger/logger.hpp"

void ClientState::render()
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
    if (connection_.estabilishConnection(address, port) == Status::Error)
    {
        LOG_INFO() << "Connection failed";
        exit(-1);
    }

    const std::string& name = form_values.at("name");
    connection_.send(name);
    LOG_INFO() << "Extracted name: " << name.c_str();

    int new_line_index = 0;
    receiver_thread_ = std::make_unique<std::thread>(
        [this, &chat_window, &new_line_index]()
        {
            while (true)
            {
                auto [buffer, status] = connection_.receive();
                if (status == Status::Error)
                {
                    return 0;
                }
                mvwprintw(chat_window.get(), 1 + new_line_index, 1, "%s", buffer.c_str());
                new_line_index++;
                box(chat_window.get(), 0, 0);
                wrefresh(chat_window.get());
            }
        });

    while (true)
    {
        char buffer[256] = {0};
        getInput(buffer, input_window, chat_window, new_line_index, name);
        if (connection_.send(buffer) == Status::Error)
        {
            exit(-1);
        }
    }

}

ClientState::~ClientState()
{
    if (receiver_thread_ != nullptr)
    {
        receiver_thread_->join();
    }
    endwin();
}
