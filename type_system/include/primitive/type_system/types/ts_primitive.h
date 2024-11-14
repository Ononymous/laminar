//
// Created by Lukas Brand on 24.01.2023.
//

#ifndef TYPE_SYSTEM_TS_PRIMITIVE_H
#define TYPE_SYSTEM_TS_PRIMITIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type_system/ts_types.h"

void set_boolean(struct ts_value* value, bool b);
void set_byte(struct ts_value* value, int8_t b);
void set_short(struct ts_value* value, int16_t s);
void set_integer(struct ts_value* value, int32_t i);
void set_long(struct ts_value* value, int64_t l);
void set_unsigned_byte(struct ts_value* value, uint8_t b);
void set_unsigned_short(struct ts_value* value, uint16_t s);
void set_unsigned_integer(struct ts_value* value, uint32_t i);
void set_unsigned_long(struct ts_value* value, uint64_t l);
void set_float(struct ts_value* value, float f);
void set_double(struct ts_value* value, double d);
void set_timestamp(struct ts_value* value, int64_t t);
void set_prim_string(struct ts_value* value, const char *str);
void set_prim_large_string(struct ts_value* value, const char* str);
void set_prim_2d_double_array(struct ts_value* value, const double **data);
void set_prim_double_array(struct ts_value* value, const double *data);

void get_boolean(bool* b, const struct ts_value* value);
void get_byte(int8_t* b, const struct ts_value* value);
void get_short(int16_t* s, const struct ts_value* value);
void get_integer(int32_t* i, const struct ts_value* value);
void get_long(int64_t* l, const struct ts_value* value);
void get_unsigned_byte(uint8_t* b, const struct ts_value* value);
void get_unsigned_short(uint16_t* s, const struct ts_value* value);
void get_unsigned_integer(uint32_t* i, const struct ts_value* value);
void get_unsigned_long(uint64_t* l, const struct ts_value* value);
void get_float(float* f, const struct ts_value* value);
void get_double(double* d, const struct ts_value* value);
void get_timestamp(int64_t* t, const struct ts_value* value);
void get_prim_string(char *str, int size, const struct ts_value* value);
void get_prim_large_string(char *str, int size, const struct ts_value* value);
void get_prim_2d_double_array(double **data, const struct ts_value* value);
void get_prim_double_array(double *data, const struct ts_value* value);

struct ts_value* value_from_boolean(bool b);
struct ts_value* value_from_byte(int8_t b);
struct ts_value* value_from_short(int16_t s);
struct ts_value* value_from_integer(int32_t i);
struct ts_value* value_from_long(int64_t l);
struct ts_value* value_from_unsigned_byte(uint8_t b);
struct ts_value* value_from_unsigned_short(uint16_t s);
struct ts_value* value_from_unsigned_integer(uint32_t i);
struct ts_value* value_from_unsigned_long(uint64_t l);
struct ts_value* value_from_float(float f);
struct ts_value* value_from_double(double d);
struct ts_value* value_from_timestamp(int64_t t);
struct ts_value* value_from_prim_string(const char *str);
struct ts_value* value_from_prim_large_string(const char *str);
struct ts_value* value_from_prim_2d_double_array(const double** data);
struct ts_value* value_from_prim_double_array(const double* data);

bool value_to_boolean(const struct ts_value* value);
int8_t value_to_byte(const struct ts_value* value);
int16_t value_to_short(const struct ts_value* value);
int32_t value_to_integer(const struct ts_value* value);
int64_t value_to_long(const struct ts_value* value);
uint8_t value_to_unsigned_byte(const struct ts_value* value);
uint16_t value_to_unsigned_short(const struct ts_value* value);
uint32_t value_to_unsigned_integer(const struct ts_value* value);
uint64_t value_to_unsigned_long(const struct ts_value* value);
float value_to_float(const struct ts_value* value);
double value_to_double(const struct ts_value* value);
int64_t value_to_timestamp(const struct ts_value* value);
char *value_to_prim_string(const struct ts_value* value);
char *value_to_prim_large_string(const struct ts_value* value);
double **value_to_prim_2d_double_array(const struct ts_value* value);
double *value_to_prim_double_array(const struct ts_value* value);

#ifdef __cplusplus
}
#endif

#endif // TYPE_SYSTEM_TS_PRIMITIVE_H
