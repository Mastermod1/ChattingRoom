#pragma once

#include <ncurses.h>

#include <functional>
#include <memory>
#include <string>
#include <arpa/inet.h>


in_addr_t convertStringAddressToUint32(const std::string& address);

void getInput(char* buffer, std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& input_window,
              std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>& chat_window, int& new_line_index,
              const std::string& name);
