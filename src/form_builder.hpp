#pragma once

#include <curses.h>
#include <form.h>

#include <functional>
#include <string>
#include <vector>

#include "form_wrapper.hpp"

class FormBuilder
{
  public:
    FormBuilder(WINDOW* win) : win_(win) {}

    FormBuilder& addField(const std::function<void(FIELD*)>& callback)
    {
        FIELD* field = new_field(1, 20, fields_.size(), 0, 0, 0);
        fields_.push_back(field);
        callback(field);
        return *this;
    };

    FormBuilder& addLabelField(
        const std::string& label, const std::function<void(FIELD*)>& callback = [](FIELD*) {})
    {
        FIELD* field = new_field(1, label.size(), fields_.size(), 0, 0, 0);
        fields_.push_back(field);
        set_field_buffer(field, 0, label.c_str());
        callback(field);
        return *this;
    }

    FormBuilder& addInputField(const std::function<void(FIELD*)>& callback = [](FIELD*) {})
    {
        FIELD* field = new_field(1, 20, fields_.size(), 0, 0, 0);
        fields_.push_back(field);
        set_field_back(field, A_UNDERLINE);
        field_opts_off(field, O_AUTOSKIP);
        callback(field);
        return *this;
    }

    FormWrapper getForm() { return FormWrapper(fields_, win_); }

    ~FormBuilder() {}

  private:
    WINDOW* win_;
    std::vector<FIELD*> fields_;
};
