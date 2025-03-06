#pragma once

#include <ncurses.h>
#include <string>

class Window
{
  public:
    Window(int height, int width, int y, int x) : height_(height), width_(width), y_(y), x_(x)
    {
        win_ = newwin(height_, width_, y_, x_);
    }
    operator WINDOW*() { return win_; }
    void refresh()
    {
        box(win_, 0, 0);
        wrefresh(win_);
    }
    void print(std::string str)
    {
        mvwprintw(win_, cursor_, 1, "%s\n", str.c_str());
        if (cursor_ < (std::size_t)height_ - 1)
            cursor_++;
    }
    ~Window() { delwin(win_); }

  private:
    int height_;
    int width_;
    int y_;
    int x_;
    std::size_t cursor_ = 1;
    WINDOW* win_ = nullptr;
};
