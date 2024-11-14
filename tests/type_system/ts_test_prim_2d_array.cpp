#include "../../df_interface.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/ts_types.h"

#include <iostream>
#include <string>

// SAMPLE APPLICATION
// (a + b) * c
// MUL(ADD(a, b), c)

int main() {
    int ns = 1; // Namespace
    int hd = 1; // Host device

    laminar_init();

    // Set up single device on localhost
    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot-apps/build/bin/");

    // Nodes

    add_node(ns, hd, 1, {.category = DF_ARITHMETIC, .operation = DF_ARITH_ADDITION});       // a + b

    // Inputs
    add_operand(ns, hd, 2); // a
    add_operand(ns, hd, 3); // b

    int id_a = 2, id_b = 3;

    // Edges
    subscribe("1:1:0", "1:2"); // ADD[0] <-- a
    subscribe("1:1:1", "1:3"); // ADD[1] <-- b

    // Optional: Autogenerate diagram
    std::cout << graphviz_representation() << std::endl;
    // exit(0);

    laminar_setup();

    // Example: (10x10 1s + 10x10 2s)
    struct ts_value value_a{};
    const int rows = TS_PRIM_2D_DOUBLE_ARRAY_ROWS;
    const int cols = TS_PRIM_2D_DOUBLE_ARRAY_COLS;
    double** op1array = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        op1array[i] = new double[cols];
        for (int j = 0; j < cols; ++j) {
            op1array[i][j] = 1.0;
        }
    }
    set_prim_2d_double_array(&value_a, (const double**)op1array);
    operand op_a(&value_a, 1);
    fire_operand(ns, id_a, &op_a, true);
    std::cout << "Fired: " << value_as_string(&op_a.operand_value) << std::endl;

    struct ts_value value_b{};
    double** op2array = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        op2array[i] = new double[cols];
        for (int j = 0; j < cols; ++j) {
            op2array[i][j] = 2.0;
        }
    }
    set_prim_2d_double_array(&value_b, (const double**)op2array);
    operand op_b(&value_b, 1);
    fire_operand(ns, id_b, &op_b, true);
    std::cout << "Fired: " << value_as_string(&op_b.operand_value) << std::endl;

    std::cout << "Operands fired" << std::endl;

    operand result;
    int err = get_result(ns, 1, &result, 1);
    
    std::cout << "Result obtained" << std::endl;

    if (err < 0) {
        std::cout << "Failed to read the result " << std::endl;
    }

    std::cout << "Result: " << value_as_string(&result.operand_value) << std::endl;
}
