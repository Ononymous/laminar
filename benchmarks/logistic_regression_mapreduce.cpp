#include <unistd.h>

#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "../df_interface.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/ts_types.h"

// Defaults
#define NUM_MAPPERS 10
#define POINTS_PER_SHARD 50000
#define NUM_ITERATIONS 5

#define ARGS "m:p:"
char *Usage = "logistic_regression_mapreduce -m num_mappers -p data_points_per_mapper\n";

using namespace std;

// Function to generate random data points
void generateData(string folder, int coordinates, int num_mappers, int points_per_shard) {
    printf("Generating %d files with %d points each...\n", num_mappers, points_per_shard);
    mt19937 gen(1234);
    uniform_real_distribution<> distrib(-1000.0, 1000.0);

    mkdir(folder.c_str(), 0666);
    for (int i = 1; i <= num_mappers; i++) {
        std::string file_path = folder + "/shard_" + std::to_string(i) + ".txt";
        ofstream file(file_path);
        if (!file.is_open()) {
            std::cout << "Could not open generation file" << std::endl;
            exit(1);
        }
        for (int j = 0; j < points_per_shard; j++) {
            for (int k = 0; k < coordinates; k++) {
                double val = 0;
                if (k == coordinates - 1) {
                    // label for logistic regression
                    val = rand() % 2;
                }
                else {
                    val = distrib(gen);
                }
                file << val << (k < coordinates - 1 ? "," : "\n");
            }
        }
        file.close();
    }
}

// 10 D Space
int main(int argc, char **argv) {
    int opt, num_mappers = 0, points_per_shard = 0;

    // Loop through the arguments
    while ((opt = getopt(argc, argv, ARGS)) != -1) {
        switch (opt) {
        case 'm':
            num_mappers = std::atoi(optarg); // Convert option argument to int
            break;
        case 'p':
            points_per_shard = std::atoi(optarg); // Convert option argument to int
            break;
        case '?': // In case of unknown option
            fprintf(stderr,
                "unrecognized command %c\n",
                    (char)opt);
			fprintf(stderr,"usage: %s",Usage);
            return 1;
        }
    }
    if (num_mappers == 0)
        num_mappers = NUM_MAPPERS;
    if (points_per_shard == 0)
        points_per_shard = POINTS_PER_SHARD;

    laminar_init();
    std::vector<double> latencies;

    // Set up single device on localhost
    set_host(1);
    add_host(1, "localhost", "/home/centos/laminar/");

    // namespace 1 : fixed operands (all on host 1)
    add_operand(1, 1, 1); // beta param data
    add_operand(1, 1, 2); // file_path
    add_operand(1, 1, 3); // learning_rate

    for (int i=1; i<=num_mappers; ++i) {
        // namespace 2 : mapper shard_num operands (all on host 1)
        add_operand(2, 1, i);

        // namespace 3 : mapper nodes
        add_node(3, 1, i, {.category = DF_CUSTOM, .operation = LOGISTIC_REGRESSION_MAPPER_PRIM});
    }

    // namespace 4 : reducer node
    add_node(4, 1, 1, {.category = DF_CUSTOM, .operation = LOGISTIC_REGRESSION_REDUCER_PRIM});

    // mapper subscriptions
    for (int i=1; i<=num_mappers; ++i) {
        subscribe(3, i, 0, 1, 1); // MAPPER[0] <-- BETA_PARAMS_DATA
        subscribe(3, i, 1, 1, 2); // MAPPER[1] <-- FILE_PATH
        subscribe(3, i, 2, 2, i); // MAPPER[2] <-- SHARD_NUM
    }

    // reducer subscriptions
    subscribe(4, 1, 0, 1, 1); // REDUCER[0] <-- BETA_PARAMS_DATA
    subscribe(4, 1, 1, 1, 3); // REDUCER[1] <-- LEARNING_RATE
    for (int i=1; i<=num_mappers; ++i) {
        subscribe(4, 1, i+1, 3, i); // REDUCER[i+1] <-- MAPPER
    }

    // Optional: Autogenerate diagram
    // std::cout << graphviz_representation() << std::endl;
    // exit(1);

    laminar_setup();

    // Generate random data points
    // Make sure the temp folder is on a subpath of the woof path of the host
    std::string folder = "/home/centos/laminar/tmp_data";
    generateData(folder, TS_PRIM_DOUBLE_ARRAY_SIZE, num_mappers, points_per_shard);
    double* beta = new double[TS_PRIM_DOUBLE_ARRAY_SIZE];
    for (int i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        beta[i] = 1.0;
    }
    std::cout << "Beta Params generated." << std::endl;
    std::cout << "Mappers: " << num_mappers << std::endl;
    std::cout << "Iterations: " << NUM_ITERATIONS << std::endl;

    struct ts_value value_beta{};
    set_prim_double_array(&value_beta, (const double*)beta);
    
    struct ts_value value_file_path{};
    set_prim_string(&value_file_path, folder.c_str());

    struct ts_value* learning_rate = value_from_double(0.01);
    
    for (int iteration = 1; iteration <= NUM_ITERATIONS; iteration++) {
        std::cout << "\nITERATION # " << iteration << std::endl;

        operand op_file_path(&value_file_path, iteration);
        // fire_operand(1, 2, &op_file_path);

        for (int i=1; i<=num_mappers; ++i) {
            struct ts_value* value_shard_num = value_from_integer(i);
            operand op_shard_num(value_shard_num, iteration);
            fire_operand(2, i, &op_shard_num);
            value_deep_delete(value_shard_num);
        }

        operand op_beta_params(&value_beta, iteration);
        // std::cout << "Beta Params : " << value_as_string(&op_beta_params.operand_value) << std::endl;
        fire_operand(1, 1, &op_beta_params);

        operand op_learning_rate(learning_rate, iteration);
        // std::cout << "File Path : " << value_to_prim_string(&op_file_path.operand_value) << std::endl;
        fire_operand(1, 2, &op_file_path);

        const auto now = std::chrono::high_resolution_clock::now();
        auto start_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                             now.time_since_epoch()).count();
        // std::cout << "Learning Rate : " << value_as_string(&op_learning_rate.operand_value) << std::endl;
        fire_operand(1, 3, &op_learning_rate);

        // get result from reducer
        operand result;
        int err = get_result(4, 1, &result, iteration);
        if (err < 0) {
            std::cout << "Failed to read the result after iteration " << iteration << std::endl;
        }
        // std::cout << "Updated Beta Params: " << value_as_string(&result.operand_value) << std::endl;

        auto end_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double latency_s = (double)(end_time - start_time) / 1e9;
        latencies.push_back(latency_s);

        // update beta params for next iteration
        for(int i=0; i<TS_PRIM_DOUBLE_ARRAY_SIZE; i++){
            beta[i] = result.operand_value.value.ts_prim_double_array[i];
        }
        set_prim_double_array(&value_beta, (const double*)beta);
    }

    std::cout << "\n===== STATS =====" << std::endl;
    for (double latency : latencies) {
        std::cout << "Iteration length (s): " << latency << std::endl;
    }
    double sum = std::accumulate(latencies.begin(), latencies.end(), 0.0);
    double avg_latency = sum / latencies.size();
    std::cout << "AVERAGE (s) with " << num_mappers << " mappers and " << points_per_shard << " points per mapper: " << avg_latency << std::endl;

    return 0;
}