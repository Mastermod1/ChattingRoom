
#pragma once

#include <memory>
#include "src/view_states/state.hpp"

enum class DisplayState
{
    MainMenu,
    Connect,
    Client,
    HostMenu,
    Host,
    Exit,
    AfterExit,
};

class StateFactory
{
  public:
    static std::unique_ptr<State> get(const DisplayState state);
};
