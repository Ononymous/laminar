//
// Created by Lukas Brand on 06.06.2023.
//
#include "type_system/types/ts_error.h"

#include <stdlib.h>

void set_uninitialized(struct ts_value* value) {
    value->type = TS_UNINITIALIZED;
    value->value.ts_int = 0;
}

void set_unknown(struct ts_value* value) {
    value->type = TS_UNKNOWN;
    value->value.ts_int = 0;
}

void set_error(struct ts_value* value, uint32_t e) {
    value->type = TS_ERROR;
    value->value.ts_unsigned_int = e;
}

void set_exception(struct ts_value* value, uint32_t e) {
    value->type = TS_EXCEPTION;
    value->value.ts_unsigned_int = e;
}


void get_error(uint32_t* e, const struct ts_value* value) {
    *e = value->value.ts_unsigned_int;
}

void get_exception(uint32_t* e, const struct ts_value* value) {
    *e = value->value.ts_unsigned_int;
}


struct ts_value* value_from_uninitialized() {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    set_uninitialized(value);
    return value;
}

struct ts_value* value_from_unknown() {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    set_unknown(value);
    return value;
}

struct ts_value* value_from_error(uint32_t e) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    set_error(value, e);
    return value;
}

struct ts_value* value_from_exception(uint32_t e) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    set_exception(value, e);
    return value;
}


uint32_t value_to_error(const struct ts_value* value) {
    return value->value.ts_unsigned_int;
}

uint32_t value_to_exception(const struct ts_value* value) {
    return value->value.ts_unsigned_int;
}
