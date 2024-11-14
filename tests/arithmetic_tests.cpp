#include "../df_interface.h"
#include "test_utils.h"
#include "tests.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_matrix.h"
#include "type_system/types/ts_primitive.h"

#include <iostream>
#include <string>
#include <vector>

void simple_arithmetic() {
    TEST("Simple arithmetic")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    int ns = 1;

    const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
    add_node(ns, 1, 1, addition);

    add_operand(ns, 1, 2);
    add_operand(ns, 1, 3);

    subscribe(ns, 1, 0, ns, 2);
    subscribe(ns, 1, 1, ns, 3);

    laminar_setup();

    // 1 + 1 == 2
    struct ts_value* op1_value = value_from_double(1);
    operand op1(op1_value, 1);
    fire_operand(ns, 2, &op1);
    fire_operand(ns, 3, &op1);
    value_deep_delete(op1_value);

    get_result(ns, 1, &op1, 1);

    ASSERT_EQ(op1.operand_value.value.ts_double, 2, "1 + 1 == 2")

    END_TEST()
}

void complex_arithmetic() {
    TEST("Complex arithmetic")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    int ns = 1;

    const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
    add_node(ns, 1, 1, addition);

    add_operand(ns, 1, 2);
    add_operand(ns, 1, 3);

    subscribe(ns, 1, 0, ns, 2);
    subscribe(ns, 1, 1, ns, 3);

    laminar_setup();

    // 1 + 1 == 2
    struct ts_value* op1_value = value_from_double(1);
    operand op1(op1_value, 1);
    fire_operand(ns, 2, &op1);
    fire_operand(ns, 3, &op1);
    value_deep_delete(op1_value);

    // 2 + 2 == 4
    struct ts_value* op2_value = value_from_double(2);
    operand op2(op2_value, 2);
    fire_operand(ns, 2, &op2);
    fire_operand(ns, 3, &op2);
    value_deep_delete(op2_value);

    // 3 + 3 == 6
    // Receive two inputs on a before receiving inputs on b
    struct ts_value* op3_value = value_from_double(3);
    operand op3(op3_value, 3);
    struct ts_value* op4_value = value_from_double(3);
    operand op4(op4_value, 4);
    fire_operand(ns, 2, &op3);
    fire_operand(ns, 2, &op4);
    fire_operand(ns, 3, &op3);
    fire_operand(ns, 3, &op4);
    value_deep_delete(op3_value);
    value_deep_delete(op4_value);


    std::vector<operand> v;
    unsigned long iters = 4;
    for (unsigned long i = 1; i <= iters; i++) {
        get_result(ns, 1, &op1, i);
        v.push_back(op1);
    }

    // // Expected: 2 4 6 6
    // std::cout << "OUTPUTS: ";
    // for (auto& i : v) {
    //     std::cout << i.itr << ": " << i->value << " | ";
    // }
    // std::cout << std::endl;

    // Note: duplicates are allowed (e.g., [2, 2, 4, 6, 6] is valid as long as internal sequence numbers are correct)
    std::vector<unsigned long> expected = {0, 2, 4, 6, 6};
    unsigned long prev_itr = 1;
    for (auto& op : v) {
        ASSERT(op.itr == prev_itr || op.itr == prev_itr + 1, "Iteration increases monotonically")
        ASSERT_EQ(op.operand_value.value.ts_double, expected[op.itr], "Ensure value is correct for each iteration")
        prev_itr = op.itr;
    }

    END_TEST()
}

void stream_arithmetic() {
    TEST("Stream arithmetic")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    int ns = 1;

    const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
    add_node(ns, 1, 1, addition);

    add_operand(ns, 1, 2);
    add_operand(ns, 1, 3);
    add_operand(ns, 1, 4);
    add_operand(ns, 1, 5);

    subscribe(ns, 1, 0, ns, 2);
    subscribe(ns, 1, 1, ns, 3);
    subscribe(ns, 1, 2, ns, 4);
    subscribe(ns, 1, 3, ns, 5);

    laminar_setup();

    unsigned long iters = 20;
    double a = 1.0;
    double b = 2.0;
    double c = 3.0;
    double d = 4.0;

    for (unsigned long i = 1; i <= iters; i++) {
        struct ts_value* op_value = value_from_double(a);
        operand op(op_value, i);
        fire_operand(ns, 2, &op);
        op_value->value.ts_double = b;
        op.operand_value = *op_value;
        fire_operand(ns, 3, &op);
        op_value->value.ts_double = c;
        op.operand_value = *op_value;
        fire_operand(ns, 4, &op);
        op_value->value.ts_double = d;
        op.operand_value = *op_value;
        fire_operand(ns, 5, &op);
        value_deep_delete(op_value);
    }

    operand result;
    get_result(ns, 1, &result, iters);

    ASSERT_EQ(result.operand_value.value.ts_double, a + b + c + d, "Final result should be a + b + c + d")
    ASSERT_EQ(result.itr, iters, "Final result itr should be last iteration")

    std::vector<operand> v;
    operand op;

    for (unsigned long i = 1; i <= iters; i++) {
        get_result(ns, 1, &op, i);
        v.push_back(op);
    }

    // TODO: Additional checks for correctness (monotonicity, etc.)

    // std::cout << "OUTPUTS: ";
    // for (auto& i : v) {
    //     std::cout << i.itr << ": " << i->value << " | ";
    // }
    // std::cout << std::endl;

    END_TEST()
}

void quadratic_test() {
    TEST("Quadratic test")
    double a = 2, b = 5, c = 2;

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    int ns = 1;

    const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
    const struct df_operation subtraction = {DF_ARITHMETIC, DF_ARITH_SUBTRACTION};
    const struct df_operation multiplication = {DF_ARITHMETIC, DF_ARITH_MULTIPLICATION};
    const struct df_operation division = {DF_ARITHMETIC, DF_ARITH_DIVISION};
    const struct df_operation sqrt = {DF_ARITHMETIC, DF_ARITH_SQRT};
    add_node(ns, 1, 1, division);
    add_node(ns, 1, 2, multiplication); // 2 * a
    add_node(ns, 1, 3, multiplication); // -1 * b
    add_node(ns, 1, 4, addition);       // -b + sqrt(b^2 - 4ac)
    add_node(ns, 1, 5, sqrt);           // sqrt(b^2 - 4ac)
    add_node(ns, 1, 6, subtraction);    // b^2 - 4ac
    add_node(ns, 1, 7, multiplication); // b ^ 2
    add_node(ns, 1, 8, multiplication); // 4 * a * c

    add_operand(ns, 1, 9);  // a
    add_operand(ns, 1, 10); // b
    add_operand(ns, 1, 11); // c
    add_operand(ns, 1, 12); // 2 for 2 * a
    add_operand(ns, 1, 13); // -1 for -1 * b
    add_operand(ns, 1, 14); // 4 for 4 * a * c

    subscribe(ns, 1, 0, ns, 4);
    subscribe(ns, 1, 1, ns, 2);
    subscribe(ns, 2, 0, ns, 9);
    subscribe(ns, 2, 1, ns, 12);
    subscribe(ns, 3, 0, ns, 13);
    subscribe(ns, 3, 1, ns, 10);
    subscribe(ns, 4, 0, ns, 5);
    subscribe(ns, 4, 1, ns, 3);
    subscribe(ns, 5, 0, ns, 6);
    subscribe(ns, 6, 0, ns, 7);
    subscribe(ns, 6, 1, ns, 8);
    subscribe(ns, 7, 0, ns, 10);
    subscribe(ns, 7, 1, ns, 10);
    subscribe(ns, 8, 0, ns, 9);
    subscribe(ns, 8, 1, ns, 11);
    subscribe(ns, 8, 2, ns, 14);

    laminar_setup();

    struct ts_value* op_value = value_from_double(a);
    operand op(op_value);
    fire_operand(ns, 9, &op);
    op_value->value.ts_double = b;
    op.operand_value = *op_value;
    fire_operand(ns, 10, &op);
    op_value->value.ts_double = c;
    op.operand_value = *op_value;
    fire_operand(ns, 11, &op);
    op_value->value.ts_double = 2;
    op.operand_value = *op_value;
    fire_operand(ns, 12, &op);
    op_value->value.ts_double = -1;
    op.operand_value = *op_value;
    fire_operand(ns, 13, &op);
    op_value->value.ts_double = 4;
    op.operand_value = *op_value;
    fire_operand(ns, 14, &op);
    value_deep_delete(op_value);

    // operand result;
    // woof_get(p + ".output.4", &result, 1);
    // std::cout << "node #4" << " result = " << result->value << std::endl;
    // for (int i = 14; i > 0; i--) {
    //     woof_get(p + ".output." + std::to_string(i), &result, 1);
    //     std::cout << "node #" << i << " result = " << result->value << std::endl;
    // }

    get_result(ns, 1, &op, 1);
    ASSERT_EQ(op.operand_value.value.ts_double, -0.5, "Final result should be -0.5")

    END_TEST()
}

void stream_quadratic_test() {
    TEST("Stream quadratic test")
    double a = 2, b = 5, c = 2;

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    int ns = 1;

    const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
    const struct df_operation subtraction = {DF_ARITHMETIC, DF_ARITH_SUBTRACTION};
    const struct df_operation multiplication = {DF_ARITHMETIC, DF_ARITH_MULTIPLICATION};
    const struct df_operation division = {DF_ARITHMETIC, DF_ARITH_DIVISION};
    const struct df_operation sqrt = {DF_ARITHMETIC, DF_ARITH_SQRT};
    add_node(ns, 1, 1, division);
    add_node(ns, 1, 2, multiplication); // 2 * a
    add_node(ns, 1, 3, multiplication); // -1 * b
    add_node(ns, 1, 4, addition);       // -b + sqrt(b^2 - 4ac)
    add_node(ns, 1, 5, sqrt);           // sqrt(b^2 - 4ac)
    add_node(ns, 1, 6, subtraction);    // b^2 - 4ac
    add_node(ns, 1, 7, multiplication); // b ^ 2
    add_node(ns, 1, 8, multiplication); // 4 * a * c

    add_operand(ns, 1, 9);  // a
    add_operand(ns, 1, 10); // b
    add_operand(ns, 1, 11); // c
    add_operand(ns, 1, 12); // 2 for 2 * a
    add_operand(ns, 1, 13); // -1 for -1 * b
    add_operand(ns, 1, 14); // 4 for 4 * a * c

    subscribe(ns, 1, 0, ns, 4);
    subscribe(ns, 1, 1, ns, 2);
    subscribe(ns, 2, 0, ns, 9);
    subscribe(ns, 2, 1, ns, 12);
    subscribe(ns, 3, 0, ns, 13);
    subscribe(ns, 3, 1, ns, 10);
    subscribe(ns, 4, 0, ns, 5);
    subscribe(ns, 4, 1, ns, 3);
    subscribe(ns, 5, 0, ns, 6);
    subscribe(ns, 6, 0, ns, 7);
    subscribe(ns, 6, 1, ns, 8);
    subscribe(ns, 7, 0, ns, 10);
    subscribe(ns, 7, 1, ns, 10);
    subscribe(ns, 8, 0, ns, 9);
    subscribe(ns, 8, 1, ns, 11);
    subscribe(ns, 8, 2, ns, 14);

    laminar_setup();

    unsigned long iters = 15;

    for (unsigned long i = 1; i <= iters; i++) {
        struct ts_value* op_value = value_from_double(a);
        operand op(op_value, i);
        fire_operand(ns, 9, &op);
        op_value->value.ts_double = b;
        op.operand_value = *op_value;
        fire_operand(ns, 10, &op);
        op_value->value.ts_double = c;
        op.operand_value = *op_value;
        fire_operand(ns, 11, &op);
        op_value->value.ts_double = 2;
        op.operand_value = *op_value;
        fire_operand(ns, 12, &op);
        op_value->value.ts_double = -1;
        op.operand_value = *op_value;
        fire_operand(ns, 13, &op);
        op_value->value.ts_double = 4;
        op.operand_value = *op_value;
        fire_operand(ns, 14, &op);
        value_deep_delete(op_value);
    }

    operand result;
    // woof_get(p + ".output.4", &result, 1);
    // std::cout << "node #4" << " result = " << result->value << std::endl;
    // for (int i = 14; i > 0; i--) {
    //     woof_get(p + ".output." + std::to_string(i), &result, 1);
    //     std::cout << "node #" << i << " result = " << result->value << std::endl;
    // }

    get_result(ns, 1, &result, iters);
    ASSERT_EQ(result.operand_value.value.ts_double, -0.5, "Final result should be -0.5")

    END_TEST()
}

std::vector<std::vector<double>> mat_test(const std::vector<std::vector<double>>& a,
                                          const std::vector<std::vector<double>>& b) {
    size_t rows_a = a.size();
    size_t cols_a = a[0].size();
    size_t rows_b = b.size();
    size_t cols_b = b[0].size();

    // Result matrix dimensions
    size_t rows_r = a.size();
    size_t cols_r = b[0].size();

    // Create operands
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            add_operand(1, 1, i * (int)cols_a + j + 1);
        }
    }

    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            add_operand(2, 1, i * (int)cols_b + j + 1);
        }
    }

    int id;
    std::string id_str;
    for (int r = 0; r < rows_r; r++) {
        for (int c = 0; c < cols_r; c++) {
            // Create addition node for intermediate products
            const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
            add_node(4, 1, r * (int)cols_r + c + 1, addition);

            // Create all multiplication nodes for one output cell
            for (int i = 0; i < cols_a; i++) {
                id = r * (int)(cols_r * cols_a) + c * (int)cols_a + i + 1;
                const struct df_operation multiplication = {DF_ARITHMETIC, DF_ARITH_MULTIPLICATION};
                add_node(3, 1, id, multiplication);
                subscribe(3, id, 0, 1, r * (int)cols_a + i + 1);
                subscribe(3, id, 1, 2, i * (int)cols_b + c + 1);

                // Connect product to be summed
                subscribe(4, r * (int)cols_r + c + 1, i, 3, id);
            }
        }
    }

    /* Run program */
    laminar_setup();

    // Write matrices to operands
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            struct ts_value* op_value = value_from_double(a[i][j]);
            operand op(op_value, 1);
            id = i * (int)cols_a + j + 1;
            fire_operand(1, id, &op);
            value_deep_delete(op_value);
        }
    }

    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            struct ts_value* op_value = value_from_double(b[i][j]);
            operand op(op_value, 1);
            id = i * (int)cols_b + j + 1;
            fire_operand(2, id, &op);
            value_deep_delete(op_value);
        }
    }

    operand op;
    std::vector<std::vector<double>> v;
    for (int r = 0; r < rows_r; r++) {
        v.push_back({});
        for (int c = 0; c < cols_r; c++) {
            id = r * (int)cols_r + c + 1;
            get_result(4, id, &op, 1);
            v[r].push_back(op.operand_value.value.ts_double);
        }
    }

    // std::cout << "OUTPUTS:" << std::endl;
    // for (auto& row : v) {
    //     for (auto& i : row) {
    //         std::cout << i << " ";
    //     }
    //     std::cout << std::endl;
    // }

    return v;
}

void mat_test_1() {
    TEST("Matrix test 1")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    // clang-format off
    std::vector<std::vector<double>> a = {
            {1, 2},
            {3, 4}
    };
    std::vector<std::vector<double>> b = {
            {5, 6},
            {7, 8}
    };
    std::vector<std::vector<double>> expected = {
            {19, 22},
            {43, 50}
    };
    // clang-format on

    std::vector<std::vector<double>> result = mat_test(a, b);

    ASSERT((result == expected), "Incorrect matrix multiplication result")

    END_TEST()
}

void mat_test_2() {
    TEST("Matrix test 2")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    // clang-format off
    std::vector<std::vector<double>> a = {
            {1, 2, 3},
            {4, 5, 6}
    };
    std::vector<std::vector<double>> b = {
            {10, 11},
            {20, 21},
            {30, 31}
    };
    std::vector<std::vector<double>> expected = {
            {140, 146},
            {320, 335}
    };
    // clang-format on

    std::vector<std::vector<double>> result = mat_test(a, b);

    ASSERT((result == expected), "Incorrect matrix multiplication result")

    END_TEST()
}

void matrix_multiplication() {
    TEST("Matrix test type system")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    int ns = 1;

    double a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    double expected[3][3] = {{30, 36, 42}, {66, 81, 96}, {102, 126, 150}};

    const struct df_operation matrix_multiplication = {DF_LINEAR_ALGEBRA, DF_LINEAR_ALGEBRA_MATRIX_MULTIPLICATION};
    add_node(ns, 1, 1, matrix_multiplication);

    add_operand(ns, 1, 2);
    add_operand(ns, 1, 3);

    subscribe(ns, 1, 0, ns, 2);
    subscribe(ns, 1, 1, ns, 3);

    laminar_setup();

    struct ts_value* op1_value = value_from_double_matrix(a, 3, 3);
    operand op1(op1_value, 1);
    write_value(&op1.operand_value);
    woof_put(generate_woof_path(OUT_WF_TYPE, ns, 2), OUTPUT_HANDLER, &op1);
    struct ts_value* op2_value = value_from_double_matrix(a, 3, 3);
    operand op2(op2_value, 1);
    write_value(&op2.operand_value);
    woof_put(generate_woof_path(OUT_WF_TYPE, ns, 3), OUTPUT_HANDLER, &op2);
    value_deep_delete(op1_value);
    value_deep_delete(op2_value);

    operand result_op;
    get_result(ns, 1, &result_op, 1);
    struct ts_value* result_value = load_value(&result_op.operand_value);

    double result[3][3];
    get_double_matrix(result, result_value);

    for (int row_index = 0; row_index < 3; ++row_index) {
        for (int column_index = 0; column_index < 3; ++column_index) {
            ASSERT_EQ(result[row_index][column_index], expected[row_index][column_index], "matrix comparison")
        }
    }

    END_TEST()
}

void arithmetic_tests() {
    //simple_arithmetic();
    //complex_arithmetic();
    stream_arithmetic();
    //quadratic_test();
    //stream_quadratic_test();
    //mat_test_1();
    //mat_test_2();
    //matrix_multiplication();
}
