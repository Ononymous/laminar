//
// Created by Lukas Brand on 20.06.2023.
//

#include "../df_interface.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_string.h"

#include <cstdlib>
#include <fstream>


void word_count();

int main() {
    word_count();

    return 0;
}

void word_count() {
    system("sudo find . -name \"laminar*\" -delete");

    laminar_reset();
    set_host(1);
    add_host(1, "127.0.0.1", "/home/centos/cspot/build/bin/");
    laminar_init();

    int laminar_namespace = 1;

    // Split string stream
    add_operand(laminar_namespace, 1, 1); // input string
    add_operand(laminar_namespace, 1, 4); // string split delimiter
    add_node(laminar_namespace, 1, 2, {DF_STRING, DF_STRING_SPLIT});
    add_node(laminar_namespace, 1, 3, {DF_INTERNAL, DF_INTERNAL_FLATTEN});

    subscribe(laminar_namespace, 2, 0, laminar_namespace, 1);
    subscribe(laminar_namespace, 2, 1, laminar_namespace, 4);
    subscribe(laminar_namespace, 3, 0, laminar_namespace, 2);


    // Count string stream
    // add_operand(laminar_namespace, 1, 3);
    // add_node(laminar_namespace, 1, 4, {});

    laminar_setup();

    // Read file
    std::ifstream input("../../dataflow_multiwoof/benchmarks/dspbench_data/books2.dat");

    // Emit lines
    size_t iteration = 1;
    for (std::string line; getline(input, line);) {
        ts_value* const delimiter_value = value_from_string("\\W+");
        write_value(delimiter_value);
        operand delimiter_operand(delimiter_value, iteration);
        fire_operand(laminar_namespace, 4, &delimiter_operand);
        value_deep_delete(delimiter_value);

        ts_value* const string_value = value_from_string(line.c_str());
        write_value(string_value);
        operand line_operand(string_value, iteration++);
        fire_operand(laminar_namespace, 1, &line_operand);
        value_deep_delete(string_value);
    }

    std::cout << "Done emitting" << std::endl;

    bool cont = true;
    size_t i = 1;
    while (cont) {
        operand result;
        get_result(laminar_namespace, 3, &result, i++);
        ts_value* const result_value = load_value(&result.operand_value);

        char* const result_string = value_to_string(result_value);
        if (strcmp(result_string, "CSPOTEND") == 0) {
            cont = false;
        }
        std::cout << result_string << " itr:" << result.itr << std::endl;
        free(result_string);

    }
}
