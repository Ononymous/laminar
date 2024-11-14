//
// Created by Lukas Brand on 06.06.2023.
//

#ifndef TYPE_SYSTEM_TS_ERROR_H
#define TYPE_SYSTEM_TS_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type_system/ts_types.h"

void set_uninitialized(struct ts_value* value);
void set_unknown(struct ts_value* value);
void set_error(struct ts_value* value, uint32_t e);
void set_exception(struct ts_value* value, uint32_t e);

void get_error(uint32_t* e, const struct ts_value* value);
void get_exception(uint32_t* e, const struct ts_value* value);

struct ts_value* value_from_uninitialized();
struct ts_value* value_from_unknown();
struct ts_value* value_from_error(uint32_t e);
struct ts_value* value_from_exception(uint32_t e);

uint32_t value_to_error(const struct ts_value* value);
uint32_t value_to_exception(const struct ts_value* value);

#ifdef __cplusplus
}
#endif

#endif // TYPE_SYSTEM_TS_ERROR_H
