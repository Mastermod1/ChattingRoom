
#include <memory>

#include "context.hpp"
#include "helpers.hpp"
#include "state_factory.hpp"

int main()
{
    const auto context = Context::create(StateFactory::get(DisplayState::MainMenu));
    context->setContextForState();

    bool has_next_view = true;
    do
    {
        has_next_view = context->render();
    } while (has_next_view);

}
