#include "../df_interface.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_matrix.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

int main() {
    int ns = 1; // Namespace
    system("sudo find . -name \"lmr*\" -delete");
    laminar_reset(); /* reset setup data structures */

    set_host(2);
    int curr_host_id = 2;
    add_host(1, "172.31.26.11", "/home/ubuntu/laminar/build/bin");
    add_host(2, "172.31.31.200", "/home/ubuntu/laminar/build/bin");
    laminar_init();

    const struct df_operation parse = {DF_INTERNAL, DF_INTERNAL_NOOP};
    add_node(ns, 1, 1, parse);
    add_operand(ns, 1, 2);

    subscribe(ns, 1, 0, ns, 2);
    laminar_setup();

    if (curr_host_id == 1){
        uint8_t array[] = {1, 2, 3};
        struct ts_value* operand_value = value_from_unsigned_byte_array(array, 3);
        operand op(operand_value, 1);
        write_value(&op.operand_value);
        fire_operand(ns, 2, &op);
        value_deep_delete(operand_value);


        operand result;
        get_result(ns, 2, &result, 1);
        ts_value* loaded_result = load_value(&result.operand_value);

        uint8_t result_array[3];
        get_unsigned_byte_array(result_array, loaded_result);

        for(int i = 0; i < sizeof(result_array); i++){
            std::cout << (int)result_array[i] << std::endl;
        }
    }
    else{
        operand result;
        get_result(ns, 2, &result, 1);
        ts_value* loaded_result = load_value(&result.operand_value);

        uint8_t result_array[3];
        get_unsigned_byte_array(result_array, loaded_result);

        for(int i = 0; i < sizeof(result_array); i++){
            std::cout << (int)result_array[i] << std::endl;
        }
    }

    return 0;
}