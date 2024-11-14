#include "../df_interface.h"
#include "type_system/types/ts_primitive.h"

#include <iostream>
#include <string>

// SAMPLE APPLICATION
// (a**2 + b**2 + c**2)
// SUM_OF_SQUARES(a, b, c)

// The custom function is defined in df_custom_operation.c 
// custom function assumes that the input and output are ints. 

int main() {
    int ns = 1; // Namespace
    int hd = 1; // Host device

    laminar_init();

    // Set up single device on localhost
    set_host(1);
    add_host(1, "localhost", "/home/centos/cspot-apps/build/bin/");

    // Node
    add_node(ns, hd, 1, {.category = DF_CUSTOM, .operation = SUM_OF_SQUARES});

    // Inputs
    add_operand(ns, hd, 2); // a
    add_operand(ns, hd, 3); // b
    add_operand(ns, hd, 4); // c

    int id_a = 2, id_b = 3, id_c = 4;

    // Edges
    subscribe("1:1:0", "1:2");
    subscribe("1:1:1", "1:3"); 
    subscribe("1:1:2", "1:4"); 

    // Optional: Autogenerate diagram
    // std::cout << graphviz_representation() << std::endl;
    // exit(0);

    laminar_setup();

    // Example: (1**2 + 2**2 + 3**2)
    struct ts_value value_a{};
    set_integer(&value_a, 1);
    operand op_a(&value_a, 1);
    fire_operand(ns, id_a, &op_a);

    struct ts_value value_b{};
    set_integer(&value_b, 2);
    operand op_b(&value_b, 1);
    fire_operand(ns, id_b, &op_b);

    struct ts_value value_c{};
    set_integer(&value_c, 3);
    operand op_c(&value_c, 1);
    fire_operand(ns, id_c, &op_c);

    operand result;
    int err = get_result(ns, 1, &result, 1);
    if (err < 0) {
        std::cout << "Failed to read the result " << std::endl;
    }

    std::cout << "Result: " << result.operand_value.value.ts_int << std::endl;
}
