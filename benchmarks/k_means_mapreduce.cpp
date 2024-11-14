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

#define NUM_MAPPERS 10
#define POINTS_PER_SHARD 50000
#define NUM_ITERATIONS 5

#define ARGS "m:p:"
char *Usage = "k_means_mapreduce -m num_mappers -p data_points_per_mapper\n";

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
                file << distrib(gen) << (k < coordinates - 1 ? "," : "\n");
            }
        }
        file.close();
    }
}

double** get_random_clusters(int num_clusters, int num_dimensions) {
    mt19937 gen(1234);
    uniform_real_distribution<> distrib(-1000.0, 1000.0);

    double** clusters = new double*[num_clusters];
    for (int i = 0; i < num_clusters; i++) {
        clusters[i] = new double[num_dimensions];
        for (int j = 0; j < num_dimensions; j++) {
            clusters[i][j] = distrib(gen);
        }
    }
    return clusters;
}

// 10 clusters, 10 D Space
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
    add_operand(1, 1, 1); // cluster data
    add_operand(1, 1, 2); // file_path

    for (int i=1; i<=num_mappers; ++i) {
        // namespace 2 : mapper shard_num operands (all on host 1)
        add_operand(2, 1, i);

        // namespace 3 : mapper nodes
        add_node(3, 1, i, {.category = DF_CUSTOM, .operation = KMEANS_CLUTSERING_MAPPER_PRIM});
    }

    // namespace 4 : reducer node
    add_node(4, 1, 1, {.category = DF_CUSTOM, .operation = KMEANS_CLUTSERING_REDUCER_PRIM});

    // mapper subscriptions
    for (int i=1; i<=num_mappers; ++i) {
        subscribe(3, i, 0, 1, 1); // MAPPER[0] <-- CLUSTERS_DATA
        subscribe(3, i, 1, 1, 2); // MAPPER[1] <-- FILE_PATH
        subscribe(3, i, 2, 2, i); // MAPPER[2] <-- SHARD_NUM
    }

    // reducer subscriptions
    for (int i=1; i<=num_mappers; ++i) {
        subscribe(4, 1, i-1, 3, i); // REDUCER[i-1] <-- MAPPER
    }

    // Optional: Autogenerate diagram
    // std::cout << graphviz_representation() << std::endl;
    // exit(1);

    laminar_setup();

    // Generate random data points
    // Make sure the temp folder is on a subpath of the woof path of the host
    std::string folder = "/home/centos/laminar/tmp_data";
    generateData(folder, TS_PRIM_2D_DOUBLE_ARRAY_COLS, num_mappers, points_per_shard);
    double** clusters = get_random_clusters(TS_PRIM_2D_DOUBLE_ARRAY_ROWS, TS_PRIM_2D_DOUBLE_ARRAY_COLS);
    printf("Random clusters generated.\n");
    printf("Mappers: %d\n", num_mappers);
    printf("Iterations: %d\n", NUM_ITERATIONS);

    struct ts_value value_clusters{};
    set_prim_2d_double_array(&value_clusters, (const double**)clusters);
    
    struct ts_value value_file_path{};
    set_prim_string(&value_file_path, folder.c_str());
    
    for (int iteration = 1; iteration <= NUM_ITERATIONS; iteration++){
        std::cout << "\nITERATION # " << iteration << std::endl;

        operand op_file_path(&value_file_path, iteration);
        fire_operand(1, 2, &op_file_path);

        for (int i=1; i<=num_mappers; ++i) {
            struct ts_value* value_shard_num = value_from_integer(i);
            operand op_shard_num(value_shard_num, iteration);
            fire_operand(2, i, &op_shard_num);
            value_deep_delete(value_shard_num);
        }

        operand op_clusters(&value_clusters, iteration);
        // std::cout << "Clusters : " << value_as_string(&op_clusters.operand_value) << std::endl;
        const auto now = std::chrono::high_resolution_clock::now();
        auto start_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                             now.time_since_epoch()).count();
        fire_operand(1, 1, &op_clusters);

        // get result from reducer
        operand result;
        int err = get_result(4, 1, &result, iteration);
        if (err < 0) {
            std::cout << "Failed to read the result after iteration " << iteration << std::endl;
        }
        // std::cout << "Updated Clusters: " << value_as_string(&result.operand_value) << std::endl;

        auto end_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double latency_s = (double)(end_time - start_time) / 1e9;
        latencies.push_back(latency_s);

        // update clusters
        for(int i=0; i<TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++){
            for(int j=0; j<TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++){
                clusters[i][j] = result.operand_value.value.ts_prim_2d_double_array[i][j];
            }
        }
        set_prim_2d_double_array(&value_clusters, (const double**)clusters);
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