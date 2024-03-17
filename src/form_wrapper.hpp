#pragma once

#include <form.h>

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

std::string truncate(const std::string& str)
{
    int last_letter_index = str.size() - 1;
    while (str[last_letter_index] == ' ' and last_letter_index >= 0) last_letter_index--;
    return str.substr(0, last_letter_index + 1);
}

class FormWrapper
{
  public:
    FormWrapper(const std::vector<std::string>& field_labels, WINDOW* win)
    {
        int label_index = 0;
        for (size_t i = 0; i < 2 * field_labels.size(); ++i)
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

    FormWrapper(const std::vector<FIELD*>& fields, WINDOW* win) : fields_(fields)
    {
        fields_.push_back((FIELD*)NULL);
        form_ = new_form(fields_.data());
    }

    std::unordered_map<std::string, std::string> submitFormValues()
    {
        std::unordered_map<std::string, std::string> res;
        for (const auto& [index, label] : index_to_label_name_)
        {
            const FIELD* field = fields_[index];
            const std::string field_value = truncate(field_buffer(field, 0));
            res.insert({label, field_value});
        }
        return res;
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
    std::vector<FIELD*> fields_;
    std::unordered_map<int, std::string> index_to_label_name_{{1, "name"}, {3, "address"}, {5, "port"}};
};
