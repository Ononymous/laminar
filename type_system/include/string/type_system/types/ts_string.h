//
// Created by Lukas Brand on 13.05.2023.
//

#ifndef TYPE_SYSTEM_TS_STRING_H
#define TYPE_SYSTEM_TS_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#define TS_STRING_INCLUDE true // used to enable support with other complex types

#include "type_system/ts_types.h"

void set_string(struct ts_value* value, const char* s);
void get_string(char* s, const struct ts_value* value);

struct ts_value* value_from_string(const char* s);
char* value_to_string(const struct ts_value* value);

size_t get_string_length(const struct ts_value* value);

#ifdef __cplusplus
}
#endif

#endif // TYPE_SYSTEM_TS_STRING_H
