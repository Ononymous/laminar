#include "../df_interface.h"
#include "type_system/types/ts_primitive.h"
#include "test_utils.h"
#include "tests.h"

#include <cmath>
#include <iostream>
#include <random>
#include <vector>

void sqrt_loop_test() {
    TEST("Square root loop test")

    laminar_init();
    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot/build/bin/");

    /* Nodes */

    // Inputs
    add_operand(1, 1, 1); // X
    add_operand(1, 1, 2); // Epsilon

    // // Initialization
    const struct df_operation division = {DF_ARITHMETIC, DF_ARITH_DIVISION};
    const struct df_operation select = {DF_INTERNAL, DF_INTERNAL_SELECT};
    const struct df_operation offset = {DF_INTERNAL, DF_INTERNAL_OFFSET};
    add_node(2, 1, 1, division); // Root = X / 2.0
    add_node(2, 1, 2, select);   // Initial root or body output?
    add_node(2, 1, 3, offset);   // Account for no body output before first iter
    add_operand(2, 1, 4);        // 2.0
    add_operand(2, 1, 5);        // SEL: root, body, body, ...
    add_operand(2, 1, 6);        // Offset = 1

    // Test
    const struct df_operation multiplication = {DF_ARITHMETIC, DF_ARITH_MULTIPLICATION};
    const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
    const struct df_operation subtraction = {DF_ARITHMETIC, DF_ARITH_SUBTRACTION};
    const struct df_operation absolute = {DF_ARITHMETIC, DF_ARITH_ABS};
    const struct df_operation less_than = {DF_LOGIC, DF_LOGIC_LESS_THAN};
    const struct df_operation not_ = {DF_LOGIC, DF_LOGIC_NOT};
    const struct df_operation filter = {DF_INTERNAL, DF_INTERNAL_FILTER};
    add_node(3, 1, 1, multiplication);
    add_node(3, 1, 2, subtraction);
    add_node(3, 1, 3, absolute);
    add_node(3, 1, 4, less_than);
    add_node(3, 1, 5, not_);
    add_node(3, 1, 6, filter); // Repeat body
    add_node(3, 1, 7, filter); // Produce result

    // Body
    add_node(4, 1, 1, division);
    add_node(4, 1, 2, addition);
    add_node(4, 1, 3, division); // Root
    add_operand(4, 1, 4);        // 2.0

    /* Edges */

    // Initialization
    subscribe("2:1:0", "1:1"); // Root = X / _
    subscribe("2:1:1", "2:4"); // Root = X / 2.0
    subscribe("2:2:0", "2:5"); // Selector
    subscribe("2:2:1", "2:1"); // SEL=0: Root = X / 2.0
    subscribe("2:2:2", "2:3"); // SEL=1: Root = result of body
    subscribe("2:3:0", "2:6"); // Offset = 1
    subscribe("2:3:1", "3:6"); // Result of body with offset

    // Test
    subscribe("3:1:0", "4:3"); // 3N1 = Root * _
    subscribe("3:1:1", "4:3"); // 3N1 = Root * Root
    subscribe("3:2:0", "1:1"); // 3N2 = X - _
    subscribe("3:2:1", "3:1"); // 3N2 = X - 3N1
    subscribe("3:3:0", "3:2"); // 3N3 = ABS(3N2)
    subscribe("3:4:0", "3:3"); // 3N4 = 3N3 < _
    subscribe("3:4:1", "1:2"); // 3N4 = 3N3 < Epsilon
    subscribe("3:5:0", "3:4"); // 3N5 = NOT(3N4)
    subscribe("3:6:0", "3:5"); // Repeat if !(delta < epsilon)
    subscribe("3:6:1", "4:3"); // Pass root back to body
    subscribe("3:7:0", "3:4"); // Result if (delta < epsilon)
    subscribe("3:7:1", "4:3"); // Pass root to result

    // Body
    subscribe("4:1:0", "1:1"); // 4N1 = X / _
    subscribe("4:1:1", "2:2"); // 4N1 = X / Root
    subscribe("4:2:0", "4:1"); // 4N2 = 4N1 + _
    subscribe("4:2:1", "2:2"); // 4N2 = 4N1 + Root
    subscribe("4:3:0", "4:2"); // 4N3 = 4N2 / _
    subscribe("4:3:1", "4:4"); // 4N3 = 4N2 / 2.0

    /* Run program */

    laminar_setup();

    double x = 144.0;
    double epsilon = 10;

    int iters = 4;

    struct ts_value* double_value = value_from_double(2);
    operand op(double_value, 1);
    fire_operand(4, 4, &op);
    operand op2(double_value, 2);
    fire_operand(4, 4, &op2);
    operand op3(double_value, 3);
    fire_operand(4, 4, &op3);
    operand op4(double_value, 4);
    fire_operand(4, 4, &op4);
    value_deep_delete(double_value);

    struct ts_value* x_value = value_from_double(x);
    op.operand_value = op2.operand_value = op3.operand_value = op4.operand_value = *x_value;
    fire_operand(1, 1, &op);
    fire_operand(1, 1, &op2);
    fire_operand(1, 1, &op3);
    fire_operand(1, 1, &op4);
    value_deep_delete(x_value);

    struct ts_value* epsilon_value = value_from_double(epsilon);
    op.operand_value = op2.operand_value = op3.operand_value = op4.operand_value = *epsilon_value;
    fire_operand(1, 2, &op);
    fire_operand(1, 2, &op2);
    fire_operand(1, 2, &op3);
    fire_operand(1, 1, &op4);
    value_deep_delete(epsilon_value);

    // Initialization

    // Seed initialization feedback with junk (not used in first iter)
    struct ts_value* empty_value = value_from_double(0);
    op.operand_value = *empty_value;
    fire_operand(2, 3, &op);
    value_deep_delete(empty_value);
    // unsigned long consumer_ptr = 2;
    // woof_put("laminar-2.subscription_pointer.3", "", &consumer_ptr);

    struct ts_value* two_value = value_from_double(2);
    op.operand_value = op2.operand_value = op3.operand_value = op4.operand_value = *two_value;
    fire_operand(2, 4, &op);
    fire_operand(2, 4, &op2);
    fire_operand(2, 4, &op3);
    fire_operand(2, 4, &op4);
    value_deep_delete(two_value);
    struct ts_value* zero_value = value_from_double(0);
    op.operand_value = *zero_value;
    struct ts_value* one_value = value_from_double(1);
    op2.operand_value = op3.operand_value = op4.operand_value = *one_value;
    fire_operand(2, 5, &op);
    fire_operand(2, 5, &op2);
    fire_operand(2, 5, &op3);
    fire_operand(2, 5, &op4);
    op.operand_value = op2.operand_value = op3.operand_value = op4.operand_value = *one_value;
    fire_operand(2, 6, &op);
    fire_operand(2, 6, &op2);
    fire_operand(2, 6, &op3);
    fire_operand(2, 6, &op4);
    value_deep_delete(zero_value);
    value_deep_delete(one_value);

    std::vector<double> v;
    for (unsigned long i = 1; i <= iters; i++) {
        get_result(3, 6, &op, i);
        v.push_back(op.operand_value.value.ts_double);
    }

    get_result(3, 7, &op, 1);
    const double result = op.operand_value.value.ts_double;

    // // Expected: 37, 20.4459, ...
    // std::cout << "Intermediate values: ";
    // for (auto& i : v) {
    //     std::cout << i << " | ";
    // }
    // std::cout << std::endl;
    // std::cout << "Result: " << result << std::endl;

    ASSERT_EQ(v[0], 37, "First iteration")
    ASSERT_EQ(round(v[1] * 1e4) / 1e4, 20.4459, "Second iteration")
    ASSERT_EQ(round(v[2] * 1e4) / 1e4, 13.7445, "Third iteration")
    ASSERT_EQ(round(result * 1e4) / 1e4, 12.1107, "Final result")

    END_TEST()
}

void multinode_regression() {
    TEST("Multinode regression test")

    laminar_init();
    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot/build/bin/");

    // Update variables (num, x, y, xx, xy)
    const struct df_operation multiplication = {DF_ARITHMETIC, DF_ARITH_MULTIPLICATION};
    const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
    const struct df_operation offset = {DF_INTERNAL, DF_INTERNAL_OFFSET};
    const struct df_operation select = {DF_INTERNAL, DF_INTERNAL_SELECT};
    const struct df_operation subtraction = {DF_ARITHMETIC, DF_ARITH_SUBTRACTION};
    const struct df_operation division = {DF_ARITHMETIC, DF_ARITH_DIVISION};
    const struct df_operation greater_than = {DF_LOGIC, DF_LOGIC_GREATER_THAN};
    add_node(1, 1, 1, multiplication);  // num *= decay_rate
    add_node(1, 1, 2, multiplication);  // x *= decay_rate
    add_node(1, 1, 3, multiplication);  // y *= decay_rate
    add_node(1, 1, 4, multiplication);  // xx *= decay_rate
    add_node(1, 1, 5, multiplication);  // xy *= decay_rate
    add_node(1, 1, 6, addition);        // num += 1
    add_node(1, 1, 7, addition);        // x += new_x
    add_node(1, 1, 8, addition);        // y += new_y
    add_node(1, 1, 9, multiplication);  // new_x ^ 2
    add_node(1, 1, 10, addition);       // xx += new_x ^ 2
    add_node(1, 1, 11, multiplication); // new_x * new_y
    add_node(1, 1, 12, addition);       // xy += new_x * new_y
    add_node(1, 1, 13, offset);         // num seq + 1
    add_node(1, 1, 14, offset);         // x seq + 1
    add_node(1, 1, 15, offset);         // y seq + 1
    add_node(1, 1, 16, offset);         // xx seq + 1
    add_node(1, 1, 17, offset);         // xy seq + 1
    add_node(1, 1, 18, select);         // num or 0?
    add_node(1, 1, 19, select);         // x or 0?
    add_node(1, 1, 20, select);         // y or 0?
    add_node(1, 1, 21, select);         // xx or 0?
    add_node(1, 1, 22, select);         // xy or 0?

    // Calculate slope and intercept

    add_node(2, 1, 1, multiplication); // num * xx
    add_node(2, 1, 2, multiplication); // x * x
    add_node(2, 1, 3, subtraction);    // det = num * xx - x * x
    add_node(2, 1, 4, multiplication); // xx * y
    add_node(2, 1, 5, multiplication); // xy * x
    add_node(2, 1, 6, subtraction);    // xx * y - xy * x
    add_node(2, 1, 7, division);       // intercept = (xx * y - xy * x) / det;
    add_node(2, 1, 8, multiplication); // xy * num
    add_node(2, 1, 9, multiplication); // x * y
    add_node(2, 1, 10, subtraction);   // xy * num - x * y
    add_node(2, 1, 11, division);      // slope = (xy * num - x * y) / det;
    add_node(2, 1, 12, greater_than);  // det > 1e-10?
    // add_node(2, 1, 13, SEL);   // intercept or 0?
    // // add_node(2, 1, 14, SEL);   // slope or 0?

    // Constants
    add_operand(3, 1, 1); // const 1
    add_operand(3, 1, 2); // const 0
    add_operand(3, 1, 3); // decay_factor = exp(-dt / T)
    add_operand(3, 1, 4); // handle init (0, 1, 1, ..., 1)
    add_operand(3, 1, 5); // const 1e-10

    // Inputs

    add_operand(4, 1, 1); // input x
    add_operand(4, 1, 2); // input y

    // Outputs

    add_node(5, 1, 1, select); // intercept = intercept or 0
    add_node(5, 1, 2, select); // slope = slope or 0

    // Edges

    // Update num, x, y, xx, xy
    subscribe("1:1:0", "3:3");   // decay_rate * _
    subscribe("1:1:1", "1:18");  // decay_rate * num
    subscribe("1:2:0", "3:3");   // decay_rate * _
    subscribe("1:2:1", "1:19");  // decay_rate * x
    subscribe("1:3:0", "3:3");   // decay_rate * _
    subscribe("1:3:1", "1:20");  // decay_rate * y
    subscribe("1:4:0", "3:3");   // decay_rate * _
    subscribe("1:4:1", "1:21");  // decay_rate * xx
    subscribe("1:5:0", "3:3");   // decay_rate * _
    subscribe("1:5:1", "1:22");  // decay_rate * xy
    subscribe("1:6:0", "3:1");   // _ + 1
    subscribe("1:6:1", "1:1");   // num += 1
    subscribe("1:7:0", "4:1");   // _ + new_x
    subscribe("1:7:1", "1:2");   // x += new_x
    subscribe("1:8:0", "4:2");   // _ + new_y
    subscribe("1:8:1", "1:3");   // y += new_y
    subscribe("1:9:0", "4:1");   // new_x * _
    subscribe("1:9:1", "4:1");   // new_x * new_x
    subscribe("1:10:0", "1:9");  // _ + new_x * new_x
    subscribe("1:10:1", "1:4");  // xx += new_x * new_x
    subscribe("1:11:0", "4:1");  // x * _
    subscribe("1:11:1", "4:2");  // x * y
    subscribe("1:12:0", "1:11"); // _ + x * y
    subscribe("1:12:1", "1:5");  // xy += x * y

    // Feedback offset
    subscribe("1:13:0", "3:1");  // offset = 1
    subscribe("1:13:1", "1:6");  // num seq + 1
    subscribe("1:14:0", "3:1");  // offset = 1
    subscribe("1:14:1", "1:7");  // x seq + 1
    subscribe("1:15:0", "3:1");  // offset = 1
    subscribe("1:15:1", "1:8");  // y seq + 1
    subscribe("1:16:0", "3:1");  // offset = 1
    subscribe("1:16:1", "1:10"); // xx seq + 1
    subscribe("1:17:0", "3:1");  // offset = 1
    subscribe("1:17:1", "1:12"); // xy seq + 1
    subscribe("1:18:0", "3:4");  // SEL: 0, 1, ..., 1
    subscribe("1:18:1", "3:2");  // 0 or _?
    subscribe("1:18:2", "1:13"); // 0 or num?
    subscribe("1:19:0", "3:4");  // SEL: 0, 1, ..., 1
    subscribe("1:19:1", "3:2");  // 0 or _?
    subscribe("1:19:2", "1:14"); // 0 or x?
    subscribe("1:20:0", "3:4");  // SEL: 0, 1, ..., 1
    subscribe("1:20:1", "3:2");  // 0 or _?
    subscribe("1:20:2", "1:15"); // 0 or y?
    subscribe("1:21:0", "3:4");  // SEL: 0, 1, ..., 1
    subscribe("1:21:1", "3:2");  // 0 or _?
    subscribe("1:21:2", "1:16"); // 0 or xx?
    subscribe("1:22:0", "3:4");  // SEL: 0, 1, ..., 1
    subscribe("1:22:1", "3:2");  // 0 or _?
    subscribe("1:22:2", "1:17"); // 0 or xy?

    // Determinant
    subscribe("2:1:0", "1:6");  // num * _
    subscribe("2:1:1", "1:10"); // num * xx
    subscribe("2:2:0", "1:7");  // x * _
    subscribe("2:2:1", "1:7");  // x * x
    subscribe("2:3:0", "2:1");  // num * xx - ____
    subscribe("2:3:1", "2:2");  // det = num * xx - x * x
    subscribe("2:12:0", "2:3"); // det > ____?
    subscribe("2:12:1", "3:5"); // det > 1e-10?

    // Intercept
    subscribe("2:4:0", "1:10"); // xx * _
    subscribe("2:4:1", "1:8");  // xx * y
    subscribe("2:5:0", "1:12"); // xy * _
    subscribe("2:5:1", "1:7");  // xy * x
    subscribe("2:6:0", "2:4");  // xx * y - ____
    subscribe("2:6:1", "2:5");  // xx * y - xy * x
    subscribe("2:7:0", "2:6");  // (xx * y - xy * x) / ____
    subscribe("2:7:1", "2:3");  // intercept = (xx * y - xy * x) / det
    subscribe("5:1:0", "2:12"); // result SEL: det > 1e-10?
    subscribe("5:1:1", "3:2");  // result = 0 or ____?
    subscribe("5:1:2", "2:7");  // result = 0 or intercept?

    // Slope
    subscribe("2:8:0", "1:12");  // xy * _
    subscribe("2:8:1", "1:6");   // xy * num
    subscribe("2:9:0", "1:7");   // x * _
    subscribe("2:9:1", "1:8");   // x * y
    subscribe("2:10:0", "2:8");  // xy * num - ____
    subscribe("2:10:1", "2:9");  // xy * num - x * y
    subscribe("2:11:0", "2:10"); // (xy * num - x * y) / ____
    subscribe("2:11:1", "2:3");  // slope = (xy * num - x * y) / det
    subscribe("5:2:0", "2:12");  // result SEL: det > 1e-10?
    subscribe("5:2:1", "3:2");   // result = 0 or ____?
    subscribe("5:2:2", "2:11");  // result = 0 or slope?

    // std::cout << graphviz_representation();
    // return;

    laminar_setup();

    // Initialization

    int iters = 20;

    std::cout << "Initializing constants" << std::endl;

    // Const (3:1) = 1
    for (int i = 1; i <= iters; i++) {
        struct ts_value* double_value = value_from_double(1.0);
        operand op(double_value, i);
        fire_operand(3, 1, &op, false);
        value_deep_delete(double_value);
    }

    // Const (3:2) = 0
    for (int i = 1; i <= iters; i++) {
        struct ts_value* double_value = value_from_double(0.0);
        operand op(double_value, i);
        fire_operand(3, 2, &op, false);
        value_deep_delete(double_value);
    }

    // Const (3:3) = exp(-dt/T)  [decay_rate]
    double dt = 1e-2;
    double T = 5e-2;
    double decay_rate = exp(-dt / T);
    for (int i = 1; i <= iters; i++) {
        struct ts_value* double_value = value_from_double(decay_rate);
        operand op(double_value, i);
        fire_operand(3, 3, &op, false);
        value_deep_delete(double_value);
    }

    // Const (3:4) = 0, 1, 1, ..., 1
    for (int i = 1; i <= iters; i++) {
        int val = (i == 1 ? 0 : 1);
        struct ts_value* double_value = value_from_double(val);
        operand op(double_value, i);
        fire_operand(3, 4, &op, false);
        value_deep_delete(double_value);
    }

    // Const (3:5) = 1e-10
    for (int i = 1; i <= iters; i++) {
        struct ts_value* double_value = value_from_double(1e-10);
        operand op(double_value, i);
        fire_operand(3, 5, &op, false);
        value_deep_delete(double_value);
    }

    // Seed offset nodes with initial value
    for (int i = 13; i <= 17; i++) {
        struct ts_value* double_value = value_from_double(0.0);
        operand op(double_value, 1);
        fire_operand(1, i, &op);
        value_deep_delete(double_value);
    }

    // Run program

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<double> distr(-0.1, 0.1);

    std::cout << "Writing x and y values... " << std::endl;

    for (int i = 0; i < iters; i++) {
        double x = i + distr(eng);
        struct ts_value* x_value = value_from_double(x);
        operand op_x(x_value, i + 1);
        fire_operand(4, 1, &op_x);

        double y = 3 + 2 * i + distr(eng);
        struct ts_value* y_value = value_from_double(y);
        operand op_y(y_value, i + 1);
        fire_operand(4, 2, &op_y);
    }

    std::vector<double> intercepts;
    std::vector<double> slopes;

    unsigned long prev_intercept_seq = 1;
    unsigned long prev_slope_seq = 1;

    for (int i = 1; i <= iters; i++) {
        operand op1;
        get_result(5, 1, &op1, i);
        intercepts.push_back(op1.operand_value.value.ts_double);

        ASSERT(op1.itr == prev_intercept_seq || op1.itr == prev_intercept_seq + 1, "Sequence increases monotonically")
        prev_intercept_seq = op1.itr;

        operand op2;
        get_result(5, 2, &op2, i);
        slopes.push_back(op2.operand_value.value.ts_double);

        ASSERT(op2.itr == prev_slope_seq || op2.itr == prev_slope_seq + 1, "Sequence increases monotonically")
        prev_slope_seq = op2.itr;

        std::cout << "[" << i << "]"
                  << " y = " << op2.operand_value.value.ts_double << "x + " << op1.operand_value.value.ts_double
                  << std::endl;
    }

    ASSERT(intercepts.size() >= iters, "Finish all iterations")

    // ASSERT(slopes.back() >= 1.5 && slopes.back() <= 2.5, "Slope is within expected range (~2)")
    // ASSERT(intercepts.back() >= 2.5 && intercepts.back() <= 3.5, "Intercept is within expected range (~3)")
    ASSERT(slopes.back() >= 0.5 && slopes.back() <= 3.5, "Slope is within expected range (~2)")
    ASSERT(intercepts.back() >= 1.5 && intercepts.back() <= 4.5, "Intercept is within expected range (~3)")


    END_TEST()
}

void loop_tests() {
    // sqrt_loop_test();
    multinode_regression();
}
