//
// Created by Lukas Brand on 13.05.2023.
//

#include "type_system/types/ts_string.h"

#include <stdlib.h>
#include <string.h>

void set_string(struct ts_value* value, const char* const s) {
    const size_t string_size = strlen(s);

    char* const string_copy = malloc(string_size + 1);
    strcpy(string_copy, s);

    struct ts_value_string string = {
        .storage_system = {.element_size = string_size + 1},
        .value = string_copy,
    };
    value->value.ts_string = string;
    value->type = TS_STRING;
}

void get_string(char* s, const struct ts_value* value) {
    strcpy(s, value->value.ts_string.value);
}


struct ts_value* value_from_string(const char* const s) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    set_string(value, s);
    return value;
}

char* value_to_string(const struct ts_value* const value) {
    const size_t string_length = strlen(value->value.ts_string.value);
    char* return_string = malloc(string_length + 1);
    strcpy(return_string, value->value.ts_string.value);
    return return_string;
}

size_t get_string_length(const struct ts_value* const value) {
    return strlen(value->value.ts_string.value);
}