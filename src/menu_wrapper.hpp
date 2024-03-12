#pragma once

#include <menu.h>

#include <string>
#include <vector>

class MenuWrapper
{
  public:
    MenuWrapper(const std::vector<std::string>& fields)
    {
        item_count_ = fields.size();
        items_ = (ITEM**)malloc((item_count_ + 1) * sizeof(ITEM*));
        for (int i = 0; i < item_count_; ++i)
        {
            items_[i] = new_item(fields.at(i).c_str(), "");
        }
        items_[item_count_] = (ITEM*)NULL;
        menu_ = new_menu(items_);
    }

    operator MENU*() { return menu_; };

    // TODO rule of five
    ~MenuWrapper()
    {
        unpost_menu(menu_);
        free_menu(menu_);
        for (int i = 0; i <= item_count_; i++) free_item(items_[i]);
    }

  private:
    MENU* menu_;
    ITEM** items_;
    size_t item_count_{0};
};
