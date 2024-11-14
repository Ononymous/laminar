//
// Created by Lukas Brand on 20.03.2023.
//
#include "df_arithmetic.h"

#include "logger_system/df_operation_logger.h"
#include "type_system/ts_type.h"

#include <math.h>
#include <stdlib.h>

int compute_add(const struct ts_value* const* operands,
                unsigned int operand_count,
                enum ts_types result_type,
                struct ts_value* result);

int compute_subtract(const struct ts_value* const* operands,
                     unsigned int operand_count,
                     enum ts_types result_type,
                     struct ts_value* result);

int compute_multiplication(const struct ts_value* const* operands,
                           unsigned int operand_count,
                           enum ts_types result_type,
                           struct ts_value* result);

int compute_division(const struct ts_value* const* operands,
                     unsigned int operand_count,
                     enum ts_types result_type,
                     struct ts_value* result);

int compute_modulo(const struct ts_value* const* operands,
                   unsigned int operand_count,
                   enum ts_types result_type,
                   struct ts_value* result);

int compute_sqrt(const struct ts_value* operand, enum ts_types result_type, struct ts_value* result);

int compute_abs(const struct ts_value* operand, enum ts_types result_type, struct ts_value* result);


static int compatible_types(const struct ts_value* const* operands,
                            unsigned int operand_count,
                            enum ts_types result_type);

static int equal_types(const struct ts_value* const* operands, unsigned int operand_count);


int df_arithmetic_operation_with_type(const enum df_arithmetic_ops arithmetic_operation,
                                      const struct ts_value* const* operands,
                                      const unsigned int operand_count,
                                      const enum ts_types result_type,
                                      struct ts_value** const result) {
    if (operand_count < 1) {
        return 0;
    }
    if (!compatible_types(operands, operand_count, result_type)) {
        return 0;
    }
    if (!equal_types(operands, operand_count)) {
        return 0;
    }
    struct ts_value* result_value = malloc(sizeof(struct ts_value));
    result_value->type = result_type;
    *result = result_value;
    switch (arithmetic_operation) {
        case DF_ARITH_ADDITION:
            return compute_add(operands, operand_count, result_type, result_value);
        case DF_ARITH_SUBTRACTION:
            return compute_subtract(operands, operand_count, result_type, result_value);
        case DF_ARITH_MULTIPLICATION:
            return compute_multiplication(operands, operand_count, result_type, result_value);
        case DF_ARITH_DIVISION:
            return compute_division(operands, operand_count, result_type, result_value);
        case DF_ARITH_MODULO:
            return compute_modulo(operands, operand_count, result_type, result_value);
        case DF_ARITH_SQRT: {
            if (operand_count != 1) {
                log_operand_count_mismatch(1, operand_count);
                return 0;
            }
            return compute_sqrt(operands[0], result_type, result_value);
        }
        case DF_ARITH_ABS:
            if (operand_count != 1) {
                log_operand_count_mismatch(1, operand_count);
                return 0;
            }
            return compute_abs(operands[0], result_type, result_value);
        default:
            log_operation_not_existing("ARITHMETIC", arithmetic_operation);
            return 0;
    }
}


int compute_add(const struct ts_value* const* operands,
                unsigned int operand_count,
                enum ts_types result_type,
                struct ts_value* const result) {
    switch (result_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            int value = 0;
            for (unsigned int i = 0; i < operand_count; i++) {
                value += operands[i]->value.ts_int;
            }
            result->value.ts_int = value;
            return 1;
        }
        case TS_LONG: {
            long value = 0;
            for (unsigned int i = 0; i < operand_count; i++) {
                value += operands[i]->value.ts_long;
            }
            result->value.ts_long = value;
            return 1;
        }
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            unsigned int value = 0;
            for (unsigned int i = 0; i < operand_count; i++) {
                value += operands[i]->value.ts_unsigned_int;
            }
            result->value.ts_unsigned_int = value;
            return 1;
        }
        case TS_UNSIGNED_LONG: {
            unsigned long value = 0;
            for (unsigned int i = 0; i < operand_count; i++) {
                value += operands[i]->value.ts_unsigned_long;
            }
            result->value.ts_unsigned_long = value;
            return 1;
        }
        case TS_DOUBLE: {
            double value = 0;
            for (unsigned int i = 0; i < operand_count; i++) {
                value += operands[i]->value.ts_double;
            }
            result->value.ts_double = value;
            return 1;
        }
        case TS_PRIM_2D_DOUBLE_ARRAY: {
            for (unsigned int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
                for (unsigned int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
                    result->value.ts_prim_2d_double_array[i][j] = 0;
                }
            }
            for (unsigned int i = 0; i < operand_count; i++) {
                for (unsigned int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; j++) {
                    for (unsigned int k = 0; k < TS_PRIM_2D_DOUBLE_ARRAY_COLS; k++) {
                        result->value.ts_prim_2d_double_array[j][k] += operands[i]->value.ts_prim_2d_double_array[j][k];
                    }
                }
            }
            return 1;
        }
        case TS_PRIM_DOUBLE_ARRAY: {
            for (unsigned int i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
                result->value.ts_prim_double_array[i] = 0;
            }
            for (unsigned int i = 0; i < operand_count; i++) {
                for (unsigned int j = 0; j < TS_PRIM_DOUBLE_ARRAY_SIZE; j++) {
                    result->value.ts_prim_double_array[j] += operands[i]->value.ts_prim_double_array[j];
                }
            }
            return 1;
        }
        default:
            return 0;
    }
}


int compute_subtract(const struct ts_value* const* operands,
                     unsigned int operand_count,
                     enum ts_types result_type,
                     struct ts_value* const result) {
    switch (result_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            int value = operands[0]->value.ts_int;
            for (unsigned int i = 1; i < operand_count; i++) {
                value -= operands[i]->value.ts_int;
            }
            result->value.ts_int = value;
            return 1;
        }
        case TS_LONG: {
            long value = operands[0]->value.ts_long;
            for (unsigned int i = 1; i < operand_count; i++) {
                value -= operands[i]->value.ts_long;
            }
            result->value.ts_long = value;
            return 1;
        }
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            unsigned int value = operands[0]->value.ts_unsigned_int;
            for (unsigned int i = 1; i < operand_count; i++) {
                value -= operands[i]->value.ts_unsigned_int;
            }
            result->value.ts_unsigned_int = value;
            return 1;
        }
        case TS_UNSIGNED_LONG: {
            unsigned long value = operands[0]->value.ts_unsigned_long;
            for (unsigned int i = 1; i < operand_count; i++) {
                value -= operands[i]->value.ts_unsigned_long;
            }
            result->value.ts_unsigned_long = value;
            return 1;
        }
        case TS_DOUBLE: {
            double value = operands[0]->value.ts_double;
            for (unsigned int i = 1; i < operand_count; i++) {
                value -= operands[i]->value.ts_double;
            }
            result->value.ts_double = value;
            return 1;
        }
        default:
            return 0;
    }
}

int compute_multiplication(const struct ts_value* const* operands,
                           unsigned int operand_count,
                           enum ts_types result_type,
                           struct ts_value* const result) {
    switch (result_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            int value = 1;
            for (unsigned int i = 0; i < operand_count; i++) {
                value *= operands[i]->value.ts_int;
            }
            result->value.ts_int = value;
            return 1;
        }
        case TS_LONG: {
            long value = 1;
            for (unsigned int i = 0; i < operand_count; i++) {
                value *= operands[i]->value.ts_long;
            }
            result->value.ts_long = value;
            return 1;
        }
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            unsigned int value = 1;
            for (unsigned int i = 0; i < operand_count; i++) {
                value *= operands[i]->value.ts_unsigned_int;
            }
            result->value.ts_unsigned_int = value;
            return 1;
        }
        case TS_UNSIGNED_LONG: {
            unsigned long value = 1;
            for (unsigned int i = 0; i < operand_count; i++) {
                value *= operands[i]->value.ts_unsigned_long;
            }
            result->value.ts_unsigned_long = value;
            return 1;
        }
        case TS_DOUBLE: {
            double value = 1;
            for (unsigned int i = 0; i < operand_count; i++) {
                value *= operands[i]->value.ts_double;
            }
            result->value.ts_double = value;
            return 1;
        }
        default:
            return 0;
    }
}

int compute_division(const struct ts_value* const* operands,
                     unsigned int operand_count,
                     enum ts_types result_type,
                     struct ts_value* const result) {
    switch (result_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            int value = operands[0]->value.ts_int;
            for (unsigned int i = 1; i < operand_count; i++) {
                value /= operands[i]->value.ts_int;
            }
            result->value.ts_int = value;
            return 1;
        }
        case TS_LONG: {
            long value = operands[0]->value.ts_long;
            for (unsigned int i = 1; i < operand_count; i++) {
                value /= operands[i]->value.ts_long;
            }
            result->value.ts_long = value;
            return 1;
        }
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            unsigned int value = operands[0]->value.ts_unsigned_int;
            for (unsigned int i = 1; i < operand_count; i++) {
                value /= operands[i]->value.ts_unsigned_int;
            }
            result->value.ts_unsigned_int = value;
            return 1;
        }
        case TS_UNSIGNED_LONG: {
            unsigned long value = operands[0]->value.ts_unsigned_long;
            for (unsigned int i = 1; i < operand_count; i++) {
                value /= operands[i]->value.ts_unsigned_long;
            }
            result->value.ts_unsigned_long = value;
            return 1;
        }
        case TS_DOUBLE: {
            double value = operands[0]->value.ts_double;
            for (unsigned int i = 1; i < operand_count; i++) {
                value /= operands[i]->value.ts_double;
            }
            result->value.ts_double = value;
            return 1;
        }
        default:
            return 0;
    }
}

int compute_modulo(const struct ts_value* const* operands,
                   unsigned int operand_count,
                   enum ts_types result_type,
                   struct ts_value* const result) {
    switch (result_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            int value = operands[0]->value.ts_int;
            for (unsigned int i = 1; i < operand_count; i++) {
                value %= operands[i]->value.ts_int;
            }
            result->value.ts_int = value;
            return 1;
        }
        case TS_LONG: {
            long value = operands[0]->value.ts_long;
            for (unsigned int i = 1; i < operand_count; i++) {
                value %= operands[i]->value.ts_long;
            }
            result->value.ts_long = value;
            return 1;
        }
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            unsigned int value = operands[0]->value.ts_unsigned_int;
            for (unsigned int i = 1; i < operand_count; i++) {
                value %= operands[i]->value.ts_unsigned_int;
            }
            result->value.ts_unsigned_int = value;
            return 1;
        }
        case TS_UNSIGNED_LONG: {
            unsigned long value = operands[0]->value.ts_unsigned_long;
            for (unsigned int i = 1; i < operand_count; i++) {
                value %= operands[i]->value.ts_unsigned_long;
            }
            result->value.ts_unsigned_long = value;
            return 1;
        }
        case TS_DOUBLE: {
            double value = operands[0]->value.ts_double;
            for (unsigned int i = 1; i < operand_count; i++) {
                value = fmod(value, operands[i]->value.ts_double);
            }
            result->value.ts_double = value;
            return 1;
        }
        default:
            return 0;
    }
}

int compute_sqrt(const struct ts_value* const operand, const enum ts_types result_type, struct ts_value* const result) {
    switch (result_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            result->value.ts_int = (int)sqrt(operand->value.ts_int);
            return 1;
        }
        case TS_LONG: {
            result->value.ts_long =
                (long)sqrtl(operand->value.ts_long); // NOLINT(cppcoreguidelines-narrowing-conversions)
            return 1;
        }
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            result->value.ts_unsigned_int = (int)sqrt(operand->value.ts_unsigned_int);
            return 1;
        }
        case TS_UNSIGNED_LONG: {
            result->value.ts_unsigned_long =
                (long)sqrtl(operand->value.ts_unsigned_long); // NOLINT(cppcoreguidelines-narrowing-conversions)
            return 1;
        }
        case TS_DOUBLE: {
            result->value.ts_double = sqrt(operand->value.ts_double);
            return 1;
        }
        default:
            return 0;
    }
}

int compute_abs(const struct ts_value* const operand, const enum ts_types result_type, struct ts_value* const result) {
    switch (result_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            result->value.ts_int = abs(operand->value.ts_int);
            return 1;
        }
        case TS_LONG: {
            result->value.ts_long = labs(operand->value.ts_long);
            return 1;
        }
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            result->value.ts_unsigned_int = operand->value.ts_unsigned_int;
            return 1;
        }
        case TS_UNSIGNED_LONG: {
            result->value.ts_unsigned_long = operand->value.ts_unsigned_long;
            return 1;
        }
        case TS_DOUBLE: {
            result->value.ts_double = fabs(operand->value.ts_double);
            return 1;
        }
        default:
            return 0;
    }
}

int is_number(const struct ts_value* value) {
    return value->type == TS_BYTE || value->type == TS_SHORT || value->type == TS_INTEGER || value->type == TS_LONG ||
           value->type == TS_UNSIGNED_BYTE || value->type == TS_UNSIGNED_SHORT || value->type == TS_UNSIGNED_INTEGER ||
           value->type == TS_UNSIGNED_LONG || value->type == TS_DOUBLE || value->type == TS_PRIM_2D_DOUBLE_ARRAY;
}


static int compatible_types(const struct ts_value* const* operands,
                            const unsigned int operand_count,
                            const enum ts_types result_type) {
    for (unsigned int i = 0; i < operand_count; i++) {
        const enum ts_types current_type = operands[i]->type;
        if (!is_number(operands[i])) {
            log_not_a_number(current_type);
            return 0;
        }
        if (current_type != result_type) {
            log_type_mismatch(result_type, current_type);
            return 0;
        }
    }
    return 1;
}

static int equal_types(const struct ts_value* const* operands, const unsigned int operand_count) {
    const enum ts_types initial_type = operands[0]->type;
    for (unsigned int i = 1; i < operand_count; i++) {
        if (operands[i]->type != initial_type) {
            return 0;
        }
    }
    return 1;
}