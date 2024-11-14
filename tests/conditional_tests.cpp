#include "../df_interface.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"
#include "test_utils.h"
#include "tests.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

const int LAMINAR_NAMESPACE = 1;

void selector_test() {
    TEST("Selector test")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    int ns = 1;

    const struct df_operation select = {DF_INTERNAL, DF_INTERNAL_SELECT};
    add_node(ns, 1, 1, select); // a or b

    add_operand(ns, 1, 2); // Selector (0 or 1)
    add_operand(ns, 1, 3); // a
    add_operand(ns, 1, 4); // b

    subscribe(ns, 1, 0, ns, 2); // Selector --> SEL:0
    subscribe(ns, 1, 1, ns, 3); // a --> SEL:1
    subscribe(ns, 1, 2, ns, 4); // b --> SEL:2

    laminar_setup();

    for (int i = 1; i <= 2; i++) {
        struct ts_value* selector = value_from_double(i - 1);
        operand op(selector, i); // Selector
        fire_operand(ns, 2, &op);
        value_deep_delete(selector);
        struct ts_value* a_value = value_from_double(10);
        operand a(a_value, i); // a
        fire_operand(ns, 3, &a);
        value_deep_delete(a_value);
        struct ts_value* b_value = value_from_double(20);
        operand b(b_value, i); // b
        fire_operand(ns, 4, &b);
        value_deep_delete(b_value);
    }


    operand r1;
    get_result(ns, 1, &r1, 1);
    ASSERT_EQ(r1.operand_value.value.ts_double, 10, "SEL = 0 -> result = a = 10")

    operand r2;
    get_result(ns, 1, &r2, 2);
    ASSERT_EQ(r2.operand_value.value.ts_double, 20, "SEL = 1 -> result = b = 20")

    END_TEST()
}

void filter_test_double() {
    TEST("Filter test double")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    const struct df_operation filter = {.category = DF_INTERNAL, .operation = DF_INTERNAL_FILTER};
    add_node(LAMINAR_NAMESPACE, 1, 1, filter);

    add_operand(LAMINAR_NAMESPACE, 1, 2); // Filter condition
    add_operand(LAMINAR_NAMESPACE, 1, 3); // Value

    subscribe(LAMINAR_NAMESPACE, 1, 0, LAMINAR_NAMESPACE, 2);
    subscribe(LAMINAR_NAMESPACE, 1, 1, LAMINAR_NAMESPACE, 3);

    laminar_setup();

    struct ts_value* condition1_value = value_from_boolean(false);
    operand filter1(condition1_value, 1);
    fire_operand(LAMINAR_NAMESPACE, 2, &filter1);
    value_deep_delete(condition1_value);
    struct ts_value* data1_value = value_from_double(1);
    operand data1(data1_value, 1);
    fire_operand(LAMINAR_NAMESPACE, 3, &data1);
    value_deep_delete(data1_value);

    struct ts_value* condition2_value = value_from_boolean(true);
    operand filter2(condition2_value, 2);
    fire_operand(LAMINAR_NAMESPACE, 2, &filter2);
    value_deep_delete(condition2_value);
    struct ts_value* data2_value = value_from_double(2);
    operand data2(data2_value, 2);
    fire_operand(LAMINAR_NAMESPACE, 3, &data2);
    value_deep_delete(data2_value);

    struct ts_value* condition3_value = value_from_boolean(true);
    operand filter3(condition3_value, 3);
    fire_operand(LAMINAR_NAMESPACE, 2, &filter3);
    value_deep_delete(condition3_value);
    struct ts_value* data3_value = value_from_double(3);
    operand data3(data3_value, 3);
    fire_operand(LAMINAR_NAMESPACE, 3, &data3);
    value_deep_delete(data3_value);

    struct ts_value* conditional4_value = value_from_boolean(true);
    operand filter4(conditional4_value, 4);
    fire_operand(LAMINAR_NAMESPACE, 2, &filter4);
    value_deep_delete(conditional4_value);
    struct ts_value* data4_value = value_from_double(4);
    operand data4(data4_value, 4);
    fire_operand(LAMINAR_NAMESPACE, 3, &data4);
    value_deep_delete(data4_value);

    struct ts_value* conditional5_value = value_from_boolean(false);
    operand filter5(conditional5_value, 5);
    fire_operand(LAMINAR_NAMESPACE, 2, &filter5);
    value_deep_delete(conditional5_value);
    struct ts_value* data5_value = value_from_double(5);
    operand data5(data5_value, 5);
    fire_operand(LAMINAR_NAMESPACE, 3, &data5);
    value_deep_delete(data5_value);


    // have not updated with get_result as filter function has variable output
    do {
        usleep(1e5);
    } while (woof_last_seq(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1)) < 2);

    std::vector<operand> v;
    operand data;
    unsigned long last = woof_last_seq(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1));
    for (unsigned long i = 1; i <= last; i++) {
        woof_get(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1), &data, i);
        v.push_back(data);
    }

    // Note: duplicates are allowed
    std::vector<unsigned long> expected = {0, 1, 0, 0, 0, 5}; // ignore values for 0, 2, 3, 4
    unsigned long prev_itr = 1;
    for (auto& op : v) {
        ASSERT(op.itr >= prev_itr, "Iteration increases monotonically")
        ASSERT_EQ(
            op.operand_value.value.ts_double, expected[op.itr], "Ensure value is correct for each iteration number")
        prev_itr = op.itr;
    }

    END_TEST()
}

void filter_test_string() {
    TEST("Filter test string")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    const struct df_operation filter = {.category = DF_INTERNAL, .operation = DF_INTERNAL_FILTER};
    add_node(LAMINAR_NAMESPACE, 1, 1, filter);

    add_operand(LAMINAR_NAMESPACE, 1, 2); // Filter condition
    add_operand(LAMINAR_NAMESPACE, 1, 3); // Value

    subscribe(LAMINAR_NAMESPACE, 1, 0, LAMINAR_NAMESPACE, 2);
    subscribe(LAMINAR_NAMESPACE, 1, 1, LAMINAR_NAMESPACE, 3);

    laminar_setup();

    {
        struct ts_value* condition1_value = value_from_boolean(false);
        operand filter1(condition1_value, 1);
        fire_operand(LAMINAR_NAMESPACE, 2, &filter1);
        value_deep_delete(condition1_value);
        struct ts_value* data1_value = value_from_string("Hello");
        operand data1(data1_value, 1);
        write_value(&data1.operand_value);
        fire_operand(LAMINAR_NAMESPACE, 3, &data1);
        value_deep_delete(data1_value);
    }

    {
        struct ts_value* condition2_value = value_from_boolean(true);
        operand filter2(condition2_value, 2);
        fire_operand(LAMINAR_NAMESPACE, 2, &filter2);
        value_deep_delete(condition2_value);
        struct ts_value* data2_value = value_from_string("Filtered1");
        operand data2(data2_value, 2);
        write_value(&data2.operand_value);
        fire_operand(LAMINAR_NAMESPACE, 3, &data2);
        value_deep_delete(data2_value);
    }

    {
        struct ts_value* condition3_value = value_from_boolean(true);
        operand filter3(condition3_value, 3);
        fire_operand(LAMINAR_NAMESPACE, 2, &filter3);
        value_deep_delete(condition3_value);
        struct ts_value* data3_value = value_from_string("Filtered2");
        operand data3(data3_value, 3);
        write_value(&data3.operand_value);
        fire_operand(LAMINAR_NAMESPACE, 3, &data3);
        value_deep_delete(data3_value);
    }

    {
        struct ts_value* conditional4_value = value_from_boolean(true);
        operand filter4(conditional4_value, 4);
        fire_operand(LAMINAR_NAMESPACE, 2, &filter4);
        value_deep_delete(conditional4_value);
        struct ts_value* data4_value = value_from_string("Filtered3");
        operand data4(data4_value, 4);
        write_value(&data4.operand_value);
        fire_operand(LAMINAR_NAMESPACE, 3, &data4);
        value_deep_delete(data4_value);
    }

    {
        struct ts_value* conditional5_value = value_from_boolean(false);
        operand filter5(conditional5_value, 5);
        fire_operand(LAMINAR_NAMESPACE, 2, &filter5);
        value_deep_delete(conditional5_value);
        struct ts_value* data5_value = value_from_string("World");
        operand data5(data5_value, 5);
        write_value(&data5.operand_value);
        fire_operand(LAMINAR_NAMESPACE, 3, &data5);
        value_deep_delete(data5_value);
    }

    // have not updated with get_result as filter function has variable output
    do {
        usleep(1e5);
    } while (woof_last_seq(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1)) < 2);

    std::vector<operand> v;
    operand data;
    unsigned long last = woof_last_seq(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1));
    for (unsigned long i = 1; i <= last; i++) {
        woof_get(generate_woof_path(OUT_WF_TYPE, LAMINAR_NAMESPACE, 1), &data, i);
        v.push_back(data);
    }

    // Note: duplicates are allowed
    std::vector<const char*> expected = {"", "Hello", "", "", "", "World"}; // ignore values for 0, 2, 3, 4
    unsigned long prev_itr = 1;
    for (auto& op : v) {
        ts_value* const value = load_value(&op.operand_value);
        char* const string = value_to_string(value);
        ASSERT(op.itr >= prev_itr, "Iteration increases monotonically")
        ASSERT(strcmp(string, expected[op.itr]) == 0, "Ensure value is correct for each iteration number")
        free(value);
        free(string);
        prev_itr = op.itr;
    }

    END_TEST()
}

void conditional_tests() {
    selector_test();
    filter_test_double();
    filter_test_string();
}
