#include "../df_interface.h"
#include "type_system/types/ts_primitive.h"
#include "test_utils.h"
#include "tests.h"

#include <iostream>
#include <unistd.h>
#include <vector>

void simple_namespace_test() {
    TEST("Simple namespace test")

    laminar_init();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");

    add_operand(1, 1, 1); // a
    add_operand(1, 1, 2); // b

    const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
    const struct df_operation multiplication = {DF_ARITHMETIC, DF_ARITH_MULTIPLICATION};
    add_node(2, 1, 1, addition);
    add_node(2, 1, 2, multiplication);
    add_node(2, 1, 3, multiplication);

    add_node(3, 1, 1, addition);
    add_node(3, 1, 2, multiplication);
    add_node(3, 1, 3, multiplication);

    add_node(4, 1, 1, addition);
    add_node(4, 1, 2, multiplication);
    add_node(4, 1, 3, multiplication);

    subscribe("2:1:0", "1:1");
    subscribe("2:1:1", "1:2");
    subscribe("2:2:0", "2:1");
    subscribe("2:2:1", "2:1");
    subscribe("2:3:0", "2:1");
    subscribe("2:3:1", "2:1");

    subscribe("3:1:0", "2:2");
    subscribe("3:1:1", "2:3");
    subscribe("3:2:0", "3:1");
    subscribe("3:2:1", "3:1");
    subscribe("3:3:0", "3:1");
    subscribe("3:3:1", "3:1");

    subscribe("4:1:0", "3:2");
    subscribe("4:1:1", "3:3");

    laminar_setup();

    struct ts_value* op_value = value_from_double(1);
    operand op(op_value);
    fire_operand(1, 1, &op);
    fire_operand(1, 2, &op);
    value_deep_delete(op_value);

    get_result(4, 1, &op, 1);

    ASSERT_EQ(op.operand_value.value.ts_double, 128, "2(2(1+1)^2)^2 = 128")

    END_TEST()
}

void namespace_tests() {
    simple_namespace_test();
}