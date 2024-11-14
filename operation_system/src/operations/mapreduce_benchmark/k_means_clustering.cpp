#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <string>

#include "mapreduce.h"

// Function to calculate Euclidean distance
double euclideanDistance(const double (*point)[TS_PRIM_2D_DOUBLE_ARRAY_COLS],
                         const double (*center)[TS_PRIM_2D_DOUBLE_ARRAY_COLS]) {
    double sum = 0.0;
    for (int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_COLS; i++) {
        sum += pow((* point)[i] - (* center)[i], 2);
    }
    return sqrt(sum);
}

void assign_cluster(const double (*point)[TS_PRIM_2D_DOUBLE_ARRAY_COLS],
                    const struct ts_value *cluster_data, const int num_clusters, struct ts_value *result) {
    double min_dist = std::numeric_limits<double>::max();
    int cluster = -1;
    for (int i = 0; i < num_clusters; i++) {
        double dist = euclideanDistance(point, &cluster_data->value.ts_prim_2d_double_array[i]);
        if (dist < min_dist) {
            min_dist = dist;
            cluster = i;
        }
    }
    // updating sum
    for (int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_COLS; i++) {
        result->value.ts_prim_2d_double_array[cluster][i] += (*point)[i];
    }
    // updating counter
    result->value.ts_prim_2d_double_array[TS_PRIM_2D_DOUBLE_ARRAY_ROWS-1][cluster] += 1;
}

int k_means_clustering_mapper_node_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type, struct ts_value* const result) {
    // Initialize the data to 0
    for (int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
        for (int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            result->value.ts_prim_2d_double_array[i][j] = 0;
        }
    }

    // Read the input data
    const struct ts_value* cluster_data = operands[0];
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
        double point[TS_PRIM_2D_DOUBLE_ARRAY_COLS];
        size_t pos = 0;
        int i = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            point[i] = std::stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            i++;
        }
        point[i] = std::stod(line);
    
        assign_cluster(&point, cluster_data, TS_PRIM_2D_DOUBLE_ARRAY_ROWS-1, result);
    }

    return 1;
}

int k_means_clustering_reducer_node_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type, struct ts_value* const result) {
    // Initialize the data to 0
    for (int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
        for (int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            result->value.ts_prim_2d_double_array[i][j] = 0;
        }
    }

    // summing up the results
    for (int op_num = 0; op_num < operand_count; op_num++) {
        struct ts_value *op = (struct ts_value *)operands[op_num];
        for (int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
            for (int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
                result->value.ts_prim_2d_double_array[i][j] += op->value.ts_prim_2d_double_array[i][j];
            }
        }
    }

    // normalizing the points
    for (int i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS-1; i++) {
        for (int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            result->value.ts_prim_2d_double_array[i][j] /= result->value.ts_prim_2d_double_array[TS_PRIM_2D_DOUBLE_ARRAY_ROWS-1][j];
        }
    }

    // setting the last row to 0
    for (int j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
        result->value.ts_prim_2d_double_array[TS_PRIM_2D_DOUBLE_ARRAY_ROWS-1][j] = 0;
    }

    return 1;
}
