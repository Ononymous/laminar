//
// Created by Lukas Brand on 17.04.2023.
//
#include "type_system/types/ts_matrix.h"

#include <stdlib.h>
#include <uuid/uuid.h>

void set_boolean_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(bool);

        bool* column_values = malloc(column_count * sizeof(bool));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((bool*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {
            .size = column_count, .type = TS_BOOLEAN, .value = column_values, .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_byte_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(int8_t);

        int8_t* column_values = malloc(column_count * sizeof(int8_t));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((int8_t*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {
            .size = column_count, .type = TS_BYTE, .value = column_values, .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_short_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(int16_t);

        int16_t* column_values = malloc(column_count * sizeof(int16_t));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((int16_t*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {
            .size = column_count, .type = TS_SHORT, .value = column_values, .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_integer_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(int32_t);

        int32_t* column_values = malloc(column_count * sizeof(int32_t));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((int32_t*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {
            .size = column_count, .type = TS_INTEGER, .value = column_values, .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_long_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(int64_t);

        int64_t* column_values = malloc(column_count * sizeof(int64_t));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((int64_t*)matrix) + row_index * column_count + column_index);
        }
        const struct ts_value_array row_value = {
            .size = column_count, .type = TS_LONG, .value = column_values, .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }

    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_unsigned_byte_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(uint8_t);

        uint8_t* column_values = malloc(column_count * sizeof(uint8_t));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((uint8_t*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {.size = column_count,
                                                 .type = TS_UNSIGNED_BYTE,
                                                 .value = column_values,
                                                 .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_unsigned_short_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(uint16_t);

        uint16_t* column_values = malloc(column_count * sizeof(uint16_t));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((uint16_t*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {.size = column_count,
                                                 .type = TS_UNSIGNED_SHORT,
                                                 .value = column_values,
                                                 .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_unsigned_integer_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(uint32_t);

        uint32_t* column_values = malloc(column_count * sizeof(uint32_t));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((uint32_t*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {.size = column_count,
                                                 .type = TS_UNSIGNED_INTEGER,
                                                 .value = column_values,
                                                 .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_unsigned_long_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(uint64_t);

        uint64_t* column_values = malloc(column_count * sizeof(uint64_t));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((uint64_t*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {.size = column_count,
                                                 .type = TS_UNSIGNED_LONG,
                                                 .value = column_values,
                                                 .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_float_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(float);

        float* column_values = malloc(column_count * sizeof(float));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((float*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {
            .size = column_count, .type = TS_FLOAT, .value = column_values, .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}

void set_double_matrix(struct ts_value* value, const void* matrix, size_t row_count, size_t column_count) {
    struct ts_storage_system rows_storage_system;
    uuid_generate(rows_storage_system.id);
    rows_storage_system.element_size = sizeof(struct ts_value_array);

    struct ts_value_array* row_values = malloc(row_count * sizeof(struct ts_value_array));
    for (size_t row_index = 0; row_index < row_count; row_index++) {
        struct ts_storage_system row_storage_system;
        uuid_generate(row_storage_system.id);
        row_storage_system.element_size = sizeof(double);

        double* column_values = malloc(column_count * sizeof(double));
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            column_values[column_index] = *(((double*)matrix) + row_index * column_count + column_index);
        }

        const struct ts_value_array row_value = {
            .size = column_count, .type = TS_DOUBLE, .value = column_values, .storage_system = row_storage_system};
        row_values[row_index] = row_value;
    }
    const struct ts_value_array rows = {
        .size = row_count, .type = TS_ARRAY, .value = row_values, .storage_system = rows_storage_system};

    value->type = TS_ARRAY;
    value->value.ts_array = rows;
}


void get_boolean_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const bool* const column_values = (bool*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((bool*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_byte_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const int8_t* const column_values = (int8_t*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((int8_t*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_short_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const int16_t* const column_values = (int16_t*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((int16_t*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_integer_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const int32_t* const column_values = (int32_t*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((int32_t*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }

    /*
     int32_t* return_matrix;
return_matrix = malloc(row_count * sizeof(int32_t*));
for (size_t row_index = 0; row_index < row_count; row_index++) {
    return_matrix[row_index] = malloc(column_count * sizeof(int32_t));
    const int32_t* const column_values = (int32_t*)(row_values[row_index].value);

for (size_t column_index = 0; column_index < column_count; column_index++) {
    return_matrix[row_index][column_index] = column_values[column_index];
}
}
     */
}

void get_long_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const int64_t* const column_values = (int64_t*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((int64_t*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_unsigned_byte_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const uint8_t* const column_values = (uint8_t*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((uint8_t*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_unsigned_short_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const uint16_t* const column_values = (uint16_t*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((uint16_t*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_unsigned_integer_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const uint32_t* const column_values = (uint32_t*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((uint32_t*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_unsigned_long_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const uint64_t* const column_values = (uint64_t*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((uint64_t*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_float_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const float* const column_values = (float*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((float*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}

void get_double_matrix(void* matrix, const struct ts_value* value) {
    const struct ts_value_array rows = value->value.ts_array;
    const size_t row_count = rows.size;
    const struct ts_value_array* const row_values = (struct ts_value_array*)rows.value;
    const size_t column_count = row_values->size;

    for (size_t row_index = 0; row_index < row_count; row_index++) {
        const double* const column_values = (double*)(row_values[row_index].value);
        for (size_t column_index = 0; column_index < column_count; column_index++) {
            *(((double*)matrix) + row_index * column_count + column_index) = column_values[column_index];
        }
    }
}


struct ts_value* value_from_boolean_matrix(const void* matrix, size_t row_count, size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_boolean_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_byte_matrix(const void* matrix, size_t row_count, size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_byte_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_short_matrix(const void* matrix, size_t row_count, size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_short_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_integer_matrix(const void* matrix, size_t row_count, size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_integer_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_long_matrix(const void* matrix, size_t row_count, size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_long_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_unsigned_byte_matrix(const void* matrix,
                                                    size_t row_count,
                                                    size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_unsigned_byte_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_unsigned_short_matrix(const void* matrix,
                                                     size_t row_count,
                                                     size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_unsigned_short_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_unsigned_integer_matrix(const void* matrix,
                                                       size_t row_count,
                                                       size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_unsigned_integer_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_unsigned_long_matrix(const void* matrix,
                                                    size_t row_count,
                                                    size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_unsigned_long_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_float_matrix(const void* matrix, size_t row_count, size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_float_matrix(value, matrix, row_count, column_count);
    return value;
}

struct ts_value* value_from_double_matrix(const void* matrix, size_t row_count, size_t column_count) {
    void* value = malloc(sizeof(struct ts_value));
    set_double_matrix(value, matrix, row_count, column_count);
    return value;
}


size_t get_row_count(const struct ts_value* value) {
    return value->value.ts_array.size;
}

size_t get_column_count(const struct ts_value* value) {
    return ((struct ts_value_array*)value->value.ts_array.value)->size;
}

enum ts_types get_matrix_type(const struct ts_value* value) {
    return ((struct ts_value_array*)value->value.ts_array.value)->type;
}