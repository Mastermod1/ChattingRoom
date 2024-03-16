#pragma once

#include <form.h>

#include <algorithm>
#include <string>
#include <vector>

class FormWrapper
{
  public:
    FormWrapper(const std::vector<std::string>& field_labels, WINDOW* win)
    {
        int label_index = 0;
        for (int i = 0; i < 2 * field_labels.size(); ++i)
        {
            FIELD* field = new_field(1, 10, i, 0, 0, 0);
            if (i % 2 == 0)
            {
                set_field_buffer(field, 0, field_labels[label_index++].c_str());
            }
            else
            {
                set_field_back(field, A_UNDERLINE);
                field_opts_off(field, O_AUTOSKIP);
            }
            fields_.push_back(field);
        }
        fields_.push_back((FIELD*)NULL);
        form_ = new_form(fields_.data());
    }

    FormWrapper(const std::vector<FIELD*> fields, WINDOW* win) : fields_(fields)
    {
        fields_.push_back((FIELD*)NULL);
        form_ = new_form(fields_.data());
    }

    operator FORM*() { return form_; };

    ~FormWrapper()
    {
        unpost_form(form_);
        free_form(form_);
        std::for_each(fields_.begin(), fields_.end(), [](FIELD* f) { free_field(f); });
        fields_.clear();
    }

  private:
    FORM* form_;
    size_t item_count_{0};
    std::vector<FIELD*> fields_;
};
