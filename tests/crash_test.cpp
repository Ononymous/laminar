#include "../df_interface.h"
#include "type_system/types/ts_primitive.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

void crash_test() {
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
    int i;
    for (i = 1; i <= 25; i++) {
        struct ts_value* op1_value = value_from_double(1);
        operand op_1(op1_value, 1);
        fire_operand(ns, 2, &op_1);
        fire_operand(ns, 3, &op_1);
        value_deep_delete(op1_value);
    }

    for (i = 1; i <= 25; i++) {
        operand result;
        int err = get_result(ns, 1, &result, i);
        if (err < 0) {
            std::cout << "Error for iteration " << i << std::endl;
        }
        std::cout << "[Iter : " << i << " ]"
                  << "[Result : " << result.operand_value.value.ts_double << " ]" << std::endl;
    }
}

int main() {
    crash_test();
}