//
// Created by Lukas Brand on 25.04.2023.
//
#include "../df_interface.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_matrix.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"
#include "test_utils.h"
#include "tests.h"

const int LAMINAR_NAMESPACE = 1;

void primitive_types() {
    TEST("Primitive types only woof")

    const char* const woof_path = "laminar-primitive-test-only-woof";

    WooFInit();
    const int is_created = WooFCreate(woof_path, sizeof(struct ts_value), 100);
    ASSERT(is_created, "woof create")

    struct ts_value* value = value_from_double(1);
    const unsigned long is_put = WooFPut(woof_path, nullptr, value);
    ASSERT_EQ(is_put, 1, "woof put")
    value_deep_delete(value);

    do {
        usleep(1e5);
    } while (WooFGetLatestSeqno(woof_path) == 0);

    struct ts_value result {};
    WooFGet(woof_path, &result, 1);

    const double result_double = value_to_double(&result);

    ASSERT_EQ(result_double, 1, "equal values")

    END_TEST()
}

void primitive_types_laminar() {
    TEST("Primitive types")

    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot-apps/build/bin/");
    laminar_init();

    add_operand(LAMINAR_NAMESPACE, 1, 1);
    laminar_setup();

    struct ts_value* operand_value = value_from_double(2);
    operand op(operand_value, 1);
    woof_put(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1), OUTPUT_HANDLER, &op);
    write_value(&op.operand_value);
    value_deep_delete(operand_value);

    do {
        usleep(1e5);
    } while (woof_last_seq(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1)) == 0);

    operand result;
    woof_get(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1), &result, 1);
    load_value(&result.operand_value, NULL);

    const double result_double = value_to_double(&result.operand_value);

    ASSERT_EQ(result_double, 2, "equal values")


    END_TEST()
}

void array_type_laminar() {
    TEST("Array type")

    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot-apps/build/bin/");

    laminar_init();

    add_operand(LAMINAR_NAMESPACE, 1, 1);
    laminar_setup();


    double array[] = {1, 2, 3};
    struct ts_value* operand_value = value_from_double_array(array, 3);
    operand op(operand_value, 1);
    woof_put(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1), OUTPUT_HANDLER, &op);
    write_value(&op.operand_value);
    value_deep_delete(operand_value);

    do {
        usleep(1e5);
    } while (woof_last_seq(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1)) == 0);

    operand result;
    woof_get(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1), &result, 1);
    load_value(&result.operand_value, NULL);

    double result_array[3];
    get_double_array(result_array, &result.operand_value);

    ASSERT_EQ(result_array[0], array[0], "equal values")
    ASSERT_EQ(result_array[1], array[1], "equal values")
    ASSERT_EQ(result_array[2], array[2], "equal values")

    END_TEST()
}

void array_type_with_op_laminar() {
    TEST("Array type with op")

    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot-apps/build/bin/");

    laminar_init();

    const struct df_operation parse = {DF_INTERNAL, DF_INTERNAL_NOOP};
    add_node(LAMINAR_NAMESPACE, 1, 1, parse);
    add_operand(LAMINAR_NAMESPACE, 1, 2);

    subscribe(LAMINAR_NAMESPACE, 1, 0, LAMINAR_NAMESPACE, 2);
    laminar_setup();


    uint8_t array[] = {1, 2, 3};
    struct ts_value* operand_value = value_from_unsigned_byte_array(array, 3);
    operand op(operand_value, 1);
    write_value(&op.operand_value);
    fire_operand(LAMINAR_NAMESPACE, 2, &op);
    value_deep_delete(operand_value);

    operand result;
    get_result(LAMINAR_NAMESPACE, 1, &result, 1);
    ts_value* loaded_result = load_value(&result.operand_value, NULL);

    uint8_t result_array[3];
    get_unsigned_byte_array(result_array, loaded_result);

    ASSERT_EQ(result_array[0], array[0], "equal values")
    ASSERT_EQ(result_array[1], array[1], "equal values")
    ASSERT_EQ(result_array[2], array[2], "equal values")

    END_TEST()
}

void string_type_laminar() {
    TEST("String type")

    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot-apps/build/bin/");
    laminar_init();

    add_operand(LAMINAR_NAMESPACE, 1, 1);
    laminar_setup();

    const char* string = "Hello World, this is a beautiful day.";

    struct ts_value* operand_value = value_from_string(string);
    operand op(operand_value, 1);
    write_value(&op.operand_value);
    fire_operand(LAMINAR_NAMESPACE, 1, &op);

    do {
        usleep(1e5);
    } while (woof_last_seq(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1)) == 0);

    operand result;
    get_result(LAMINAR_NAMESPACE, 1, &result, 1);
    ts_value* loaded_result = load_value(&result.operand_value, NULL);

    char* string_result = static_cast<char*>(malloc(get_string_length(loaded_result) + 1));
    get_string(string_result, loaded_result);

    ASSERT(strcmp(string_result, string) == 0, "equal strings")

    END_TEST()
}

void string_type_with_op_laminar() {
    TEST("String type with op")

    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot-apps/build/bin/");

    laminar_init();

    const struct df_operation parse = {DF_INTERNAL, DF_INTERNAL_NOOP};
    add_node(LAMINAR_NAMESPACE, 1, 1, parse);
    add_operand(LAMINAR_NAMESPACE, 1, 2);

    subscribe(LAMINAR_NAMESPACE, 1, 0, LAMINAR_NAMESPACE, 2);
    laminar_setup();

    const char* string = "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World "
                         "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World ";
    struct ts_value* operand_value = value_from_string(string);
    operand op(operand_value, 1);
    write_value(&op.operand_value);
    fire_operand(LAMINAR_NAMESPACE, 2, &op);
    value_deep_delete(operand_value);

    operand result;
    get_result(LAMINAR_NAMESPACE, 1, &result, 1);
    ts_value* loaded_result = load_value(&result.operand_value, NULL);

    char* result_string = value_to_string(loaded_result);

    ASSERT(strcmp(string, result_string) == 0, "equal strings")

    END_TEST()
}

void matrix_types_laminar() {
    TEST("Matrix types")

    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot-apps/build/bin/");
    laminar_init();

    add_operand(LAMINAR_NAMESPACE, 1, 1);
    laminar_setup();


    int32_t matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    struct ts_value* operand_value = value_from_integer_matrix(matrix, 3, 3);
    operand op(operand_value, 1);
    write_value(&op.operand_value);
    woof_put(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1), OUTPUT_HANDLER, &op);
    value_deep_delete(operand_value);

    do {
        usleep(1e5);
    } while (woof_last_seq(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1)) == 0);

    operand result;
    woof_get(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1), &result, 1);
    ts_value* const result_value = load_value(&result.operand_value, NULL);

    int32_t result_matrix[3][3];
    get_integer_matrix(result_matrix, result_value);

    for (int row_index = 0; row_index < 3; ++row_index) {
        for (int column_index = 0; column_index < 3; ++column_index) {
            ASSERT_EQ(result_matrix[row_index][column_index], matrix[row_index][column_index], "matrix comparison")
        }
    }

    END_TEST()
}


void types_tests() {
//    primitive_types();
//    primitive_types_laminar();
    array_type_with_op_laminar();
//    array_type_laminar();
//    string_type_laminar();
//    string_type_with_op_laminar();
//    matrix_types_laminar();
}
