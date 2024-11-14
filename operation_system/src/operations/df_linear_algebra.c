//
// Created by Lukas Brand on 17.04.2023.
//
#include "df_linear_algebra.h"

#include "logger_system/df_operation_logger.h"
#include "type_system/types/ts_matrix.h"

#include <stdlib.h>

int compute_matrix_multiplication(const struct ts_value* const* operands,
                                  unsigned int operand_count,
                                  enum ts_types result_type,
                                  struct ts_value** result);

bool is_multipliable(struct ts_value matrix_left, struct ts_value matrix_right);


int df_linear_algebra_operation(const enum df_linear_algebra_ops linear_algebra_operation,
                                const struct ts_value* const* operands,
                                const unsigned int operand_count,
                                const enum ts_types result_type,
                                struct ts_value** const result) {
    switch (linear_algebra_operation) {
        case DF_LINEAR_ALGEBRA_MATRIX_MULTIPLICATION:
            return compute_matrix_multiplication(operands, operand_count, result_type, result);
        default:
            log_operation_not_existing("DF_LINEAR_ALGEBRA", linear_algebra_operation);
            break;
    }
}

int compute_matrix_multiplication(const struct ts_value* const* operands,
                                  const unsigned int operand_count,
                                  const enum ts_types result_type,
                                  struct ts_value** const result) {
    if (operand_count < 2) {
        log_operand_count_mismatch(2, operand_count);
        return 0;
    }
    struct ts_value* matrix_left = value_deep_copy(operands[0]);

    for (int operand_index = 1; operand_index < operand_count; operand_index++) {
        const struct ts_value* matrix_right = operands[operand_index];

        if (!is_multipliable(*matrix_left, *matrix_right)) {
            return 0;
        }

        const size_t matrix_left_row_count = get_row_count(matrix_left);
        const size_t matrix_left_column_count = get_column_count(matrix_left);
        const size_t matrix_right_row_count = get_row_count(matrix_right);
        const size_t matrix_right_column_count = get_column_count(matrix_right);


        const enum ts_types matrix_type = get_matrix_type(matrix_left);
        switch (matrix_type) {
            case TS_BOOLEAN: {
                bool matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_boolean_matrix(&matrix_left_values, matrix_left);
                bool matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_boolean_matrix(&matrix_right_values, matrix_right);
                bool matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] = (bool)(matrix_result_values[i][j] +
                                                                matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_boolean_matrix(matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_BYTE: {
                int8_t matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_byte_matrix(&matrix_left_values, matrix_left);
                int8_t matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_byte_matrix(&matrix_right_values, matrix_right);
                int8_t matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] = (int8_t)(matrix_result_values[i][j] +
                                                                  matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_byte_matrix(matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_SHORT: {
                int16_t matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_short_matrix(&matrix_left_values, matrix_left);
                int16_t matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_short_matrix(&matrix_right_values, matrix_right);
                int16_t matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] =
                                (int16_t)(matrix_result_values[i][j] +
                                          matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_short_matrix(matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_INTEGER: {
                int32_t matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_integer_matrix(&matrix_left_values, matrix_left);
                int32_t matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_integer_matrix(&matrix_right_values, matrix_right);
                int32_t matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] =
                                (int32_t)(matrix_result_values[i][j] +
                                          matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_integer_matrix(matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_LONG: {
                int64_t matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_long_matrix(&matrix_left_values, matrix_left);
                int64_t matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_long_matrix(&matrix_right_values, matrix_right);
                int64_t matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] =
                                (int64_t)(matrix_result_values[i][j] +
                                          matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_long_matrix(matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_UNSIGNED_BYTE: {
                uint8_t matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_unsigned_byte_matrix(&matrix_left_values, matrix_left);
                uint8_t matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_unsigned_byte_matrix(&matrix_right_values, matrix_right);
                uint8_t matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] =
                                (uint8_t)(matrix_result_values[i][j] +
                                          matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_unsigned_byte_matrix(
                    matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_UNSIGNED_SHORT: {
                uint16_t matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_unsigned_short_matrix(&matrix_left_values, matrix_left);
                uint16_t matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_unsigned_short_matrix(&matrix_right_values, matrix_right);
                uint16_t matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] =
                                (uint16_t)(matrix_result_values[i][j] +
                                           matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_unsigned_short_matrix(
                    matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_UNSIGNED_INTEGER: {
                uint32_t matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_unsigned_integer_matrix(&matrix_left_values, matrix_left);
                uint32_t matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_unsigned_integer_matrix(&matrix_right_values, matrix_right);
                uint32_t matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] =
                                (uint32_t)(matrix_result_values[i][j] +
                                           matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_unsigned_integer_matrix(
                    matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_UNSIGNED_LONG: {
                uint64_t matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_unsigned_long_matrix(&matrix_left_values, matrix_left);
                uint64_t matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_unsigned_long_matrix(&matrix_right_values, matrix_right);
                uint64_t matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] =
                                (uint64_t)(matrix_result_values[i][j] +
                                           matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }
                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_unsigned_long_matrix(
                    matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_FLOAT: {
                float matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_float_matrix(&matrix_left_values, matrix_left);
                float matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_float_matrix(&matrix_right_values, matrix_right);
                float matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] = (float)(matrix_result_values[i][j] +
                                                                  matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }

                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_float_matrix(matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            case TS_DOUBLE: {
                double matrix_left_values[matrix_left_row_count][matrix_left_column_count];
                get_double_matrix(&matrix_left_values, matrix_left);
                double matrix_right_values[matrix_right_row_count][matrix_right_column_count];
                get_double_matrix(&matrix_right_values, matrix_right);
                double matrix_result_values[matrix_left_row_count][matrix_right_column_count];

                for (int row_index = 0; row_index < matrix_left_row_count; ++row_index) {
                    for (int column_index = 0; column_index < matrix_right_column_count; ++column_index) {
                        matrix_result_values[row_index][column_index] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int k = 0; k < 3; k++) {
                            matrix_result_values[i][j] = (double)(matrix_result_values[i][j] +
                                                                  matrix_left_values[i][k] * matrix_right_values[k][j]);
                        }
                    }
                }

                value_deep_delete(matrix_left);
                matrix_left = malloc(sizeof(struct ts_value));
                set_double_matrix(matrix_left, matrix_result_values, matrix_left_row_count, matrix_right_column_count);
            } break;
            default:
                return 0;
        }
    }
    *result = value_deep_copy(matrix_left);
    return 1;
}

/*
bool is_compatible_typed(const DF_VALUE matrix_left, const DF_VALUE matrix_right) {
    if (matrix_left.type != DF_ARRAY || matrix_right.type != DF_ARRAY) {
        return false;
    }
    const DF_VALUE_ARRAY matrix_left_rows = matrix_left.value.df_array;
    const DF_VALUE_ARRAY matrix_right_rows = matrix_right.value.df_array;
    if (matrix_left_rows.type != DF_ARRAY || matrix_right_rows.type != DF_ARRAY) {
        return false;
    }
    const size_t matrix_left_row_count = matrix_left_rows.size;
    size_t matrix_left_column_count = -1;
    for (int matrix_left_columns_index = 0; matrix_left_columns_index < matrix_left_row_count;
         matrix_left_columns_index++) {
        const DF_VALUE_ARRAY* matrix_left_row = (matrix_left_rows.value) + matrix_left_columns_index;
        if (matrix_left_column_count != -1 && matrix_left_row->size != matrix_left_column_count) {

        }
        matrix_left_column_count = matrix_left_row->size;
    }
    const size_t matrix_right_row_count = matrix_right_rows.size;
    for (int matrix_right_columns_index = 0; matrix_right_columns_index < matrix_right_row_count;
         ++matrix_right_columns_index) {
        const DF_VALUE_ARRAY * matrix_right_row = (matrix_right_rows.value) + matrix_right_columns_index;

    }

    return true;
}
*/

bool is_multipliable(const struct ts_value matrix_left, const struct ts_value matrix_right) {
    return true;
}
