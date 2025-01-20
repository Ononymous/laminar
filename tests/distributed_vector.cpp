#include "../df_interface.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_array.h"

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

int main() {
    int ns = 1; // Namespace
    system("sudo find . -name \"lmr*\" -delete");
    laminar_reset(); /* reset setup data structures */
    set_host(1);
    add_host(1, "localhost", "/cspot-device-namespace/");
    laminar_init();

    // add a node that does nothing
    add_node(ns, 1, 1, {DF_CUSTOM, VECTOR_DOT_PRODUCT});

    // single input
    add_operand(ns, 1, 2); // a
    add_operand(ns, 1, 3); // b

    subscribe(ns, 1, 0, ns, 2);
    subscribe(ns, 1, 1, ns, 3);

    laminar_setup();

    std::cout << "Vinayak1" << std::endl;

    // firing array
    int32_t a[] = {1, 2, 3};
    int32_t b[] = {2, 3, 4};
    struct ts_value* operand_value1 = value_from_integer_array(a, 3);
    std::cout << "V1" << std::endl;
    for(int i = 0; i < 3; i++){
        std::cout << ((int*)(operand_value1->value.ts_array.value))[i] << std::endl;
    }
    operand op1(operand_value1, 1);
    for(int i = 0; i < 3; i++){
        std::cout << ((int*)(op1.operand_value.value.ts_array.value))[i] << std::endl;
    }
    if(!write_value(&op1.operand_value)){
        std::cout << "Issue with write value" << std::endl;
    }
    std::cout << "V2" << std::endl;
    for(int i = 0; i < 3; i++){
        std::cout << ((int*)(op1.operand_value.value.ts_array.value))[i] << std::endl;
    }
    fire_operand(ns, 2, &op1);
    value_deep_delete(operand_value1);


    struct ts_value* operand_value2 = value_from_integer_array(b, 3);
    std::cout << "V1" << std::endl;
    for(int i = 0; i < 3; i++){
        std::cout << ((int*)(operand_value2->value.ts_array.value))[i] << std::endl;
    }
    operand op2(operand_value2, 1);
    for(int i = 0; i < 3; i++){
        std::cout << ((int*)(op2.operand_value.value.ts_array.value))[i] << std::endl;
    }
    if(!write_value(&op2.operand_value)){
        std::cout << "Issue with write value" << std::endl;
    }
    std::cout << "V2" << std::endl;
    for(int i = 0; i < 3; i++){
        std::cout << ((int*)(op2.operand_value.value.ts_array.value))[i] << std::endl;
    }
    fire_operand(ns, 3, &op2);
    value_deep_delete(operand_value2);

    std::cout << "Animesh2" << std::endl;

    // get value from woof
    operand result;
    get_result(ns, 1, &result, 1);
    std::cout << result.operand_value.type << std::endl;
    std::cout << result.itr << std::endl;
    std::cout << result.operand_value.value.ts_int << std::endl;

    return 0;
}