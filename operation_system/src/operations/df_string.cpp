//
// Created by Lukas Brand on 11.06.2023.
//

#include "df_string.h"

#include "../../../df.h"
#include "logger_system/df_logger.h"
#include "logger_system/df_operation_logger.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_string.h"

#include <cstdlib>
#include <cstring>
#include <regex>

int compute_split(const struct ts_value* string,
                  const struct ts_value* delimiter,
                  enum ts_types result_type,
                  struct ts_value* result);


int df_string_operation_with_type(const enum df_string_ops string_operation,
                                  const struct ts_value* const* operands,
                                  const unsigned int operand_count,
                                  const enum ts_types result_type,
                                  struct ts_value** const result) {
    auto* result_value = static_cast<ts_value*>(malloc(sizeof(struct ts_value)));
    *result = result_value;

    switch (string_operation) {
        case DF_STRING_SPLIT: {
            if (operand_count < 2) {
                log_operand_count_mismatch(2, operand_count);
                return 0;
            }
            return compute_split(operands[0], operands[1], result_type, result_value);
        }
        default: {
            log_operation_not_existing("STRING", string_operation);
            return 0;
        }
    }
}

int compute_split(const struct ts_value* const string,
                  const struct ts_value* const delimiter,
                  const enum ts_types result_type,
                  struct ts_value* const result) {
    const std::string string_to_split = value_to_string(string);
    const std::string regex_delimiter = value_to_string(delimiter);

    std::regex rgx(regex_delimiter);
    std::sregex_token_iterator iter(string_to_split.begin(), string_to_split.end(), rgx, -1);
    std::sregex_token_iterator end;

    std::vector<std::string> split_string;
    for (; iter != end; ++iter) {
        std::string string_part = iter->str();
        if (string_part.empty()) {
            continue;
        }
        std::transform(string_part.begin(), string_part.end(), string_part.begin(), ::toupper);
        split_string.push_back(string_part);
    }

    if (string_to_split.empty()) {
        set_string_array(result, nullptr, 0);
    } else {
        const char* string_array[string_to_split.size()];
        for (std::size_t i = 0; i < split_string.size(); i++) {
            string_array[i] = split_string[i].c_str();
        }
        set_string_array(result, string_array, split_string.size());
    }

    return true;
}

/*int main() {
    auto* const result = static_cast<ts_value* const>(malloc(sizeof(struct ts_value)));
    ts_value* const input = value_from_string("");
    ts_value* const delimiter = value_from_string("\\W+");
    compute_split(input, delimiter, TS_ARRAY, result);


}*/