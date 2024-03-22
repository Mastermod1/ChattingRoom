#include <ncurses.h>

#include <memory>

#include "context.hpp"
#include "helpers.hpp"

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    std::shared_ptr<Context> context = std::make_shared<Context>(StateFactory::get(DisplayState::MainMenu));
    context->setContextForState();

    context->render();

    endwin();
}
