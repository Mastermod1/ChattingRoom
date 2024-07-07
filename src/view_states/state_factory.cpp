#include "src/view_states/state_factory.hpp"

#include "src/view_states/client_state.hpp"
#include "src/view_states/connect_menu_state.hpp"
#include "src/view_states/host_menu_state.hpp"
#include "src/view_states/host_state.hpp"
#include "src/view_states/main_menu_state.hpp"

std::unique_ptr<State> StateFactory::get(const DisplayState state)
{
    switch (state)
    {
        case DisplayState::Connect:
            return std::make_unique<ConnectMenuState>();
        case DisplayState::Client:
            return std::make_unique<ClientState>();
        case DisplayState::MainMenu:
            return std::make_unique<MainMenuState>();
        case DisplayState::Host:
            return std::make_unique<HostState>();
        case DisplayState::HostMenu:
            return std::make_unique<HostMenuState>();
        default:
            return nullptr;
    }

    return std::make_unique<MainMenuState>();
}
