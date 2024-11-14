#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <string>

#include "mapreduce.h"

double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

int logistic_regression_mapper_node_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type, struct ts_value* const result) {
    printf("Mapper Started!\n");
    // Initialize the result to 0
    result->type = TS_PRIM_DOUBLE_ARRAY;
    for (int i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        result->value.ts_prim_double_array[i] = 0;
    }

    // Read the input data
    const struct ts_value* beta = operands[0];
    std::string data_path(value_to_prim_string(operands[1]));
    int shard_num = value_to_integer(operands[2]);

    // Accessing the points from file
    std::string file_path = data_path + "/shard_" + std::to_string(shard_num) + ".txt";
    std::ifstream file(file_path);
    std::string line;
    if (!file.is_open()) {
        log_error("Could not open input file ", file_path);
        return 1;
    }

    while (getline(file, line)) {
        double point[TS_PRIM_DOUBLE_ARRAY_SIZE];
        size_t pos = 0;
        int i = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            point[i] = std::stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            i++;
        }
        point[i] = std::stod(line);
        double y = point[TS_PRIM_DOUBLE_ARRAY_SIZE-1];
        double z = 0;
        for (size_t i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE - 1; i++){
            z += point[i] * beta->value.ts_prim_double_array[i];
        }
        double predicted = sigmoid(z);
        for (size_t i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE - 1; i++) {
            result->value.ts_prim_double_array[i] += (y - predicted) * point[i];
        }
        result->value.ts_prim_double_array[TS_PRIM_DOUBLE_ARRAY_SIZE-1] += 1;
    }

    return 1;
}

int logistic_regression_reducer_node_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type, struct ts_value* const result) {
    // Read the input data
    const struct ts_value* beta = operands[0];
    double learning_rate = value_to_double(operands[1]);

    for (size_t i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        result->value.ts_prim_double_array[i] = beta->value.ts_prim_double_array[i];;
    }

    double* global_gradient = new double[TS_PRIM_DOUBLE_ARRAY_SIZE];
    for (int i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        global_gradient[i] = 0.0;
    }

    // summing up the results
    for (int op_num = 2; op_num < operand_count; op_num++) {
        for (size_t i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
            global_gradient[i] += operands[op_num]->value.ts_prim_double_array[i];
        }
    }

    // normalizing the points
    for (size_t i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE-1; i++) {
        global_gradient[i] /= global_gradient[TS_PRIM_DOUBLE_ARRAY_SIZE-1];
    }

    // updating the beta
    for (size_t i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE-1; i++) {
        result->value.ts_prim_double_array[i] -= learning_rate * global_gradient[i];
    }

    // setting the last row to 0
    result->value.ts_prim_double_array[TS_PRIM_DOUBLE_ARRAY_SIZE-1] = 0;
    printf("Reducer worked till here!\n");

    return 1;
}
