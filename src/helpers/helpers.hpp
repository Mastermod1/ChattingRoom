#pragma once

#include <arpa/inet.h>
#include <ncurses.h>

#include <functional>
#include <memory>
#include <string>

#include "src/helpers/ncurses_wrappers/window_wrapper.hpp"

in_addr_t convertStringAddressToUint32(const std::string& address);

void getInput(char* buffer, std::unique_ptr<Window>& input_window, const std::string& name);
