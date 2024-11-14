//
// Created by Lukas Brand on 25.04.2023.
//

#ifndef TYPE_SYSTEM_TS_ARRAY_H
#define TYPE_SYSTEM_TS_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#define TS_ARRAY_INCLUDE true // used to enable support with other complex types

#include "type_system/ts_types.h"

/**
 * Store the array inside the ts_value. ts_value needs to be allocated manually.
 * @param value with stored array.
 * @param array to store.
 * @param size of array to store.
 */
void set_boolean_array(struct ts_value* value, const bool array[], size_t size);
void set_byte_array(struct ts_value* value, const int8_t array[], size_t size);
void set_short_array(struct ts_value* value, const int16_t array[], size_t size);
void set_integer_array(struct ts_value* value, const int32_t array[], size_t size);
void set_long_array(struct ts_value* value, const int64_t array[], size_t size);
void set_unsigned_byte_array(struct ts_value* value, const uint8_t array[], size_t size);
void set_unsigned_short_array(struct ts_value* value, const uint16_t array[], size_t size);
void set_unsigned_integer_array(struct ts_value* value, const uint32_t array[], size_t size);
void set_unsigned_long_array(struct ts_value* value, const uint64_t array[], size_t size);
void set_float_array(struct ts_value* value, const float array[], size_t size);
void set_double_array(struct ts_value* value, const double array[], size_t size);
void set_timestamp_array(struct ts_value* value, const int64_t array[], size_t size);

/**
 * Load the array from the ts_value.
 * @param value with stored array.
 * @param array to be loaded into.
 */
void get_boolean_array(bool array[], const struct ts_value* value);
void get_byte_array(int8_t array[], const struct ts_value* value);
void get_short_array(int16_t array[], const struct ts_value* value);
void get_integer_array(int32_t array[], const struct ts_value* value);
void get_long_array(int64_t array[], const struct ts_value* value);
void get_unsigned_byte_array(uint8_t array[], const struct ts_value* value);
void get_unsigned_short_array(uint16_t array[], const struct ts_value* value);
void get_unsigned_integer_array(uint32_t array[], const struct ts_value* value);
void get_unsigned_long_array(uint64_t array[], const struct ts_value* value);
void get_float_array(float array[], const struct ts_value* value);
void get_double_array(double array[], const struct ts_value* value);
void get_timestamp_array(int64_t array[], const struct ts_value* value);

struct ts_value* value_from_boolean_array(const bool array[], size_t size);
struct ts_value* value_from_byte_array(const int8_t array[], size_t size);
struct ts_value* value_from_short_array(const int16_t array[], size_t size);
struct ts_value* value_from_integer_array(const int32_t array[], size_t size);
struct ts_value* value_from_long_array(const int64_t array[], size_t size);
struct ts_value* value_from_unsigned_byte_array(const uint8_t array[], size_t size);
struct ts_value* value_from_unsigned_short_array(const uint16_t array[], size_t size);
struct ts_value* value_from_unsigned_integer_array(const uint32_t array[], size_t size);
struct ts_value* value_from_unsigned_long_array(const uint64_t array[], size_t size);
struct ts_value* value_from_float_array(const float array[], size_t size);
struct ts_value* value_from_double_array(const double array[], size_t size);
struct ts_value* value_from_timestamp_array(const int64_t array[], size_t size);

bool* value_to_boolean_array(const struct ts_value* value);
int8_t* value_to_byte_array(const struct ts_value* value);
int16_t* value_to_short_array(const struct ts_value* value);
int32_t* value_to_integer_array(const struct ts_value* value);
int64_t* value_to_long_array(const struct ts_value* value);
uint8_t* value_to_unsigned_byte_array(const struct ts_value* value);
uint16_t* value_to_unsigned_short_array(const struct ts_value* value);
uint32_t* value_to_unsigned_integer_array(const struct ts_value* value);
uint64_t* value_to_unsigned_long_array(const struct ts_value* value);
float* value_to_float_array(const struct ts_value* value);
double* value_to_double_array(const struct ts_value* value);
int64_t* value_to_timestamp_array(const struct ts_value* value);

#ifndef PLACEHOLDER // TS_STRING_INCLUDE
void set_string_array(struct ts_value* value, const char* array[], size_t size);
void get_string_array(char* array[], const struct ts_value* value);
struct ts_value* value_from_string_array(const char* array[], size_t size);
char** value_to_string_array(const struct ts_value* value);
#endif

size_t get_array_size(const struct ts_value* value);
enum ts_types get_array_type(const struct ts_value* value);

#ifdef __cplusplus
}
#endif

#endif // TYPE_SYSTEM_TS_ARRAY_H
