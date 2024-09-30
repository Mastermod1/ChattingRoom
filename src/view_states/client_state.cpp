#include "src/view_states/client_state.hpp"

#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <memory>
#include <thread>
#include <unordered_map>

#include "src/helpers/helpers.hpp"
#include "src/helpers/ncurses_wrappers/window_wrapper.hpp"
#include "src/logger/logger.hpp"
#include "src/view_states/context.hpp"

ClientState::ClientState()
{
    chat_window_ = std::make_unique<Window>(LINES - 4, COLS, 0, 0);
    input_window_ = std::make_unique<Window>(4, COLS, LINES - 4, 0);
    scrollok(*chat_window_, true);
    chat_window_->refresh();
    input_window_->refresh();
}

void ClientState::render()
{

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

    receiver_thread_ = std::make_unique<std::thread>(
        [this]()
        {
            while (true)
            {
                auto [buffer, status] = connection_.receive();
                if (status == Status::Error)
                {
                    return 0;
                }
                chat_window_->print(buffer);
                chat_window_->refresh();
            }
        });

    while (true)
    {
        char buffer[256] = {0};
        getInput(buffer, input_window_, name);
        chat_window_->print(buffer);
        chat_window_->refresh();
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
