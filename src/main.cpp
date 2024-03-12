#include "main_menu.hpp"

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    renderMainMenu();

    endwin();
}
