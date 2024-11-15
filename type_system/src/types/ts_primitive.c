//
// Created by Lukas Brand on 24.01.2023.
//
#ifdef ESP8266
#include "ts_primitive.h"
#else
#include "type_system/types/ts_primitive.h"
#endif

#include <stdlib.h>
#include <string.h>

void set_boolean(struct ts_value* value, bool b) {
    value->type = TS_BOOLEAN;
    value->value.ts_int = b;
}

void set_byte(struct ts_value* value, int8_t b) {
    value->type = TS_BYTE;
    value->value.ts_int = b;
}

void set_short(struct ts_value* value, int16_t s) {
    value->type = TS_SHORT;
    value->value.ts_int = s;
}

void set_integer(struct ts_value* value, int32_t i) {
    value->type = TS_INTEGER;
    value->value.ts_int = i;
}

void set_long(struct ts_value* value, int64_t l) {
    value->type = TS_LONG;
    value->value.ts_long = l;
}

void set_unsigned_byte(struct ts_value* value, uint8_t b) {
    value->type = TS_UNSIGNED_BYTE;
    value->value.ts_unsigned_int = b;
}

void set_unsigned_short(struct ts_value* value, uint16_t s) {
    value->type = TS_UNSIGNED_SHORT;
    value->value.ts_unsigned_int = s;
}

void set_unsigned_integer(struct ts_value* value, uint32_t i) {
    value->type = TS_UNSIGNED_INTEGER;
    value->value.ts_unsigned_int = i;
}

void set_unsigned_long(struct ts_value* value, uint64_t l) {
    value->type = TS_UNSIGNED_LONG;
    value->value.ts_unsigned_long = l;
}

void set_float(struct ts_value* value, float f) {
    value->type = TS_FLOAT;
    value->value.ts_float = f;
}

void set_double(struct ts_value* value, double d) {
    value->type = TS_DOUBLE;
    value->value.ts_double = d;
}

void set_timestamp(struct ts_value* value, int64_t t) {
    value->type = TS_LONG;
    value->value.ts_long = t;
}

void set_prim_string(struct ts_value* value, const char *str) {
    int len = sizeof(value->value.ts_prim_string) - 1;
    value->type = TS_PRIM_STRING;
    if(strlen(str) < len) {
	len = strlen(str);
    }
    strncpy(value->value.ts_prim_string,str,len);
    value->value.ts_prim_string[len] = 0;
}

void set_prim_large_string(struct ts_value* value, const char* str) {
    int len = sizeof(value->value.ts_prim_large_string) - 1;
    value->type = TS_PRIM_LARGE_STRING;
    if(strlen(str) < len) {
        len = strlen(str);
    }
    strncpy(value->value.ts_prim_large_string,str,len);
    value->value.ts_prim_large_string[len] = 0;
}

void set_prim_2d_double_array(struct ts_value* value, const double **data) {
    value->type = TS_PRIM_2D_DOUBLE_ARRAY;
    for(int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
        for(int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            value->value.ts_prim_2d_double_array[i][j] = data[i][j];
        }
    }
}

void set_prim_double_array(struct ts_value* value, const double *data) {
    value->type = TS_PRIM_DOUBLE_ARRAY;
    for(int i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        value->value.ts_prim_double_array[i] = data[i];
    }
}

void get_boolean(bool* b, const struct ts_value* value) {
    *b = value->value.ts_int;
}

void get_byte(int8_t* b, const struct ts_value* value) {
    *b = (int8_t)value->value.ts_int;
}

void get_short(int16_t* s, const struct ts_value* value) {
    *s = (int16_t)value->value.ts_int;
}

void get_integer(int32_t* i, const struct ts_value* value) {
    *i = value->value.ts_int;
}

void get_long(int64_t* l, const struct ts_value* value) {
    *l = value->value.ts_long;
}

void get_unsigned_byte(uint8_t* b, const struct ts_value* value) {
    *b = value->value.ts_unsigned_int;
}

void get_unsigned_short(uint16_t* s, const struct ts_value* value) {
    *s = value->value.ts_unsigned_int;
}

void get_unsigned_integer(uint32_t* i, const struct ts_value* value) {
    *i = value->value.ts_unsigned_int;
}

void get_unsigned_long(uint64_t* l, const struct ts_value* value) {
    *l = value->value.ts_unsigned_long;
}

void get_float(float* f, const struct ts_value* value) {
    *f = value->value.ts_float;
}

void get_double(double* d, const struct ts_value* value) {
    *d = value->value.ts_double;
}

void get_timestamp(int64_t* t, const struct ts_value* value) {
    *t = value->value.ts_long;
}

void get_prim_string(char *str, int size, const struct ts_value* value) {
	int len = strlen(value->value.ts_prim_string);
	if(len > size) {
		len = size;
	}
	strncpy(str,value->value.ts_prim_string,len);
}

void get_prim_large_string(char *str, int size, const struct ts_value* value) {
    int len = strlen(value->value.ts_prim_large_string);
    if(len > size) {
        len = size;
    }
    strncpy(str,value->value.ts_prim_large_string,len);
}

void get_prim_2d_double_array(double **data, const struct ts_value* value) {
    for(int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
        for(int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            data[i][j] = value->value.ts_prim_2d_double_array[i][j];
        }
    }
}

void get_prim_double_array(double *data, const struct ts_value* value) {
    for(int i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        data[i] = value->value.ts_prim_double_array[i];
    }
}

struct ts_value* value_from_boolean(bool b) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_int = b;
    value->type = TS_BOOLEAN;
    return value;
}

struct ts_value* value_from_byte(signed char b) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_int = (int)b;
    value->type = TS_BYTE;
    return value;
}

struct ts_value* value_from_short(short s) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_int = s;
    value->type = TS_SHORT;
    return value;
}

struct ts_value* value_from_integer(int i) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_int = i;
    value->type = TS_INTEGER;
    return value;
}

struct ts_value* value_from_long(int64_t l) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_long = l;
    value->type = TS_LONG;
    return value;
}

struct ts_value* value_from_unsigned_byte(unsigned char b) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_unsigned_int = b;
    value->type = TS_UNSIGNED_BYTE;
    return value;
}

struct ts_value* value_from_unsigned_short(unsigned short s) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_unsigned_int = s;
    value->type = TS_UNSIGNED_SHORT;
    return value;
}

struct ts_value* value_from_unsigned_integer(unsigned int i) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_unsigned_int = i;
    value->type = TS_UNSIGNED_INTEGER;
    return value;
}

struct ts_value* value_from_unsigned_long(uint64_t l) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_unsigned_long = l;
    value->type = TS_UNSIGNED_LONG;
    return value;
}

struct ts_value* value_from_float(float f) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_float = f;
    value->type = TS_FLOAT;
    return value;
}

struct ts_value* value_from_double(double d) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_double = d;
    value->type = TS_DOUBLE;
    return value;
}

struct ts_value* value_from_timestamp(int64_t t) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    value->value.ts_long = t;
    value->type = TS_TIMESTAMP;
    return value;
}

struct ts_value* value_from_prim_string(const char *str) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    strncpy(value->value.ts_prim_string,str,sizeof(value->value.ts_prim_string)); 
    value->type = TS_PRIM_STRING;
    return value;
}

struct ts_value* value_from_prim_large_string(const char *str) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    strncpy(value->value.ts_prim_large_string,str,sizeof(value->value.ts_prim_large_string));
    value->type = TS_PRIM_LARGE_STRING;
    return value;
}

struct ts_value* value_from_prim_2d_double_array(const double** data) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    for(int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
        for(int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            value->value.ts_prim_2d_double_array[i][j] = data[i][j];
        }
    }
    value->type = TS_PRIM_2D_DOUBLE_ARRAY;
    return value;
}

struct ts_value* value_from_prim_double_array(const double* data) {
    struct ts_value* value = malloc(sizeof(struct ts_value));
    for(int i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        value->value.ts_prim_double_array[i] = data[i];
    }
    value->type = TS_PRIM_DOUBLE_ARRAY;
    return value;
}

bool value_to_boolean(const struct ts_value* value) {
    return value->value.ts_int;
}

int8_t value_to_byte(const struct ts_value* value) {
    return (int8_t)value->value.ts_int;
}

int16_t value_to_short(const struct ts_value* value) {
    return (int16_t)value->value.ts_int;
}

int32_t value_to_integer(const struct ts_value* value) {
    return value->value.ts_int;
}

int64_t value_to_long(const struct ts_value* value) {
    return value->value.ts_long;
}

uint8_t value_to_unsigned_byte(const struct ts_value* value) {
    return value->value.ts_unsigned_int;
}

uint16_t value_to_unsigned_short(const struct ts_value* value) {
    return value->value.ts_unsigned_int;
}

uint32_t value_to_unsigned_integer(const struct ts_value* value) {
    return value->value.ts_unsigned_int;
}

uint64_t value_to_unsigned_long(const struct ts_value* value) {
    return value->value.ts_unsigned_long;
}

float value_to_float(const struct ts_value* value) {
    return value->value.ts_float;
}

double value_to_double(const struct ts_value* value) {
    return value->value.ts_double;
}

int64_t value_to_timestamp(const struct ts_value* value) {
    return value->value.ts_long;
}

char *value_to_prim_string(const struct ts_value* value) {
    return value->value.ts_prim_string;
}

char *value_to_prim_large_string(const struct ts_value* value) {
    return value->value.ts_prim_large_string;
}

double **value_to_prim_2d_double_array(const struct ts_value* value) {
    double** data = (double**)malloc(TS_PRIM_2D_DOUBLE_ARRAY_ROWS * sizeof(double*));
    if (data == NULL) {
        return NULL;
    }
    for(int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
        data[i] = (double*)malloc(TS_PRIM_2D_DOUBLE_ARRAY_COLS * sizeof(double));
        if (data[i] == NULL) {
            // Free previously allocated memory
            for (int j = 0; j < i; ++j) {
                free(data[j]);
            }
            free(data);
            return NULL;
        }
        for(int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            data[i][j] = value->value.ts_prim_2d_double_array[i][j];
        }
    }
    return data;
}

double *value_to_prim_double_array(const struct ts_value* value) {
    double* data = (double*)malloc(TS_PRIM_DOUBLE_ARRAY_SIZE * sizeof(double));
    if (data == NULL) {
        return NULL;
    }
    for(int i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        data[i] = value->value.ts_prim_double_array[i];
    }
    return data;
}
