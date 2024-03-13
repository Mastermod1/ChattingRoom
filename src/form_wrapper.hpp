#pragma once

#include <form.h>

#include <string>
#include <vector>

class FormWrapper
{
  public:
    FormWrapper(const std::vector<std::string>& fields, WINDOW* win)
    {
        item_count_ = 2 * fields.size();
        fields_ = (FIELD**)malloc((item_count_ + 1) * sizeof(FIELD*));
        int x = 0;
        for (int i = 0; i < item_count_; ++i)
        {
            if (i % 2 == 0)
            {
                fields_[i] = new_field(1, 10, i, 0, 0, 0);
                set_field_buffer(fields_[i], 0, fields[x++].c_str());
            }
            else
            {
                fields_[i] = new_field(1, 10, i, 0, 0, 0);
                set_field_back(fields_[i], A_UNDERLINE);
                field_opts_off(fields_[i], O_AUTOSKIP);
            }
        }
        fields_[item_count_] = (FIELD*)NULL;
        form_ = new_form(fields_);
    }

    operator FORM*() { return form_; };

    // TODO rule of five
    ~FormWrapper()
    {
        unpost_form(form_);
        free_form(form_);
        for (int i = 0; i <= item_count_; i++) free_field(fields_[i]);
    }

  private:
    FORM* form_;
    FIELD** fields_;
    size_t item_count_{0};
};
