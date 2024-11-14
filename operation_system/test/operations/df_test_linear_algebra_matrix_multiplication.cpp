//
// Created by Lukas Brand on 17.04.2023.
//
#include "../../src/operations/df_linear_algebra.h"
#include "catch2/catch_test_macros.hpp"
#include "type_system/types/ts_matrix.h"

#include <iostream>

TEST_CASE("linear_algebra: matrix_multiplication") {
    const enum df_linear_algebra_ops linear_algebra_op = DF_LINEAR_ALGEBRA_MATRIX_MULTIPLICATION;
    const int operand_count = 2;
    struct ts_value* result = nullptr;
    int return_value;
    SECTION("TS_INTEGER") {
        int32_t matrix_a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        int32_t matrix_b[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};

        struct ts_value op_transform{};
        set_integer_matrix(&op_transform, matrix_a, 3, 3);

        const size_t row_count = get_row_count(&op_transform);
        const size_t column_count = get_column_count(&op_transform);

        int32_t result_transform[row_count][column_count];
        get_integer_matrix(&result_transform, &op_transform);

        bool transform_equal = true;
        for (int row_index = 0; row_index < 3; ++row_index) {
            for (int column_index = 0; column_index < 3; ++column_index) {
                transform_equal =
                    transform_equal && (matrix_a[row_index][column_index] == result_transform[row_index][column_index]);
                std::cout << matrix_a[row_index][column_index] << ":" << result_transform[row_index][column_index]
                          << std::endl;
            }
        }
        REQUIRE(transform_equal == true);


        struct ts_value op1{};
        set_integer_matrix(&op1, matrix_a, 3, 3);
        struct ts_value op2{};
        set_integer_matrix(&op2, matrix_b, 3, 3);
        const struct ts_value* op_list[2] = {&op1, &op2};
        return_value =
            df_linear_algebra_operation(linear_algebra_op, op_list, operand_count, TS_UNINITIALIZED, &result);

        int32_t matrix_expected_result[3][3] = {0};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    matrix_expected_result[i][j] += matrix_a[i][k] * matrix_b[k][j];
                }
            }
        }

        REQUIRE(get_row_count(result) == 3);
        REQUIRE(get_column_count(result) == 3);
        int32_t matrix_actual_result[get_row_count(result)][get_column_count(result)];
        get_integer_matrix(matrix_actual_result, result);

        bool result_equal = true;
        for (int row_index = 0; row_index < 3; ++row_index) {
            for (int column_index = 0; column_index < 3; ++column_index) {
                result_equal = result_equal && (matrix_expected_result[row_index][column_index] ==
                                                matrix_actual_result[row_index][column_index]);
            }
        }
        REQUIRE(result_equal == true);
        REQUIRE(result != NULL);
        REQUIRE(return_value == 1);
    } //
}