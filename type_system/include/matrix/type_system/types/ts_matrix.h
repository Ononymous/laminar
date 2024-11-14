//
// Created by Lukas Brand on 17.04.2023.
//

#ifndef TYPE_SYSTEM_TS_MATRIX_H
#define TYPE_SYSTEM_TS_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type_system/ts_types.h"

void set_boolean_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_byte_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_short_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_integer_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_long_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_unsigned_byte_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_unsigned_short_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_unsigned_integer_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_unsigned_long_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_float_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);
void set_double_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count);

void get_boolean_matrix(void* matrix, const struct ts_value* value);
void get_byte_matrix(void* matrix, const struct ts_value* value);
void get_short_matrix(void* matrix, const struct ts_value* value);
void get_integer_matrix(void* matrix, const struct ts_value* value);
void get_long_matrix(void* matrix, const struct ts_value* value);
void get_unsigned_byte_matrix(void* matrix, const struct ts_value* value);
void get_unsigned_short_matrix(void* matrix, const struct ts_value* value);
void get_unsigned_integer_matrix(void* matrix, const struct ts_value* value);
void get_unsigned_long_matrix(void* matrix, const struct ts_value* value);
void get_float_matrix(void* matrix, const struct ts_value* value);
void get_double_matrix(void* matrix, const struct ts_value* value);

struct ts_value* value_from_boolean_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_byte_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_short_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_integer_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_long_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_unsigned_byte_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_unsigned_short_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_unsigned_integer_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_unsigned_long_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_float_matrix(const void* matrix, size_t row_count, size_t column_count);
struct ts_value* value_from_double_matrix(const void* matrix, size_t row_count, size_t column_count);

size_t get_row_count(const struct ts_value* value);
size_t get_column_count(const struct ts_value* value);
enum ts_types get_matrix_type(const struct ts_value* value);

#ifdef __cplusplus
}
#endif

#endif // TYPE_SYSTEM_TS_MATRIX_H
