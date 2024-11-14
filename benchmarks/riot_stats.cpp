#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../df_interface.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_matrix.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"

/**
 * The STATS configuration for mHealth dataset can be found here:
 * https://github.com/dream-lab/riot-bench/blob/master/modules/tasks/src/main/resources/tasks_FIT.properties
 * 
 * Configure properties for this benchmark in operation_system/src/operations/riot_benchmark/riot_config.h
 * 
 * Use the process_riot_stats.py in scripts directory to parse the riot_sink.csv
 * 
 * NOTE: The RIoT Bench paper states that the mhealth data originally has an input throughput of 500 msg/sec.
*/

#define INPUT_THROUGHPUT 500    // default input throughput
#define INPUT_DURATION 10   // default input duration (s)

#define ARGS "m:d:"
char *Usage = "riot_stats -m messages_per_second -d duration_in_seconds\n";

/**
 * Keep the messages_per_second as a factor of 1000000.
 * The benchmark will stop before duration_in_seconds if it runs out of data.
*/
int main(int argc, char **argv) {
    system("rm -f ./riot_sink.csv");
    system("rm -f ./riot_aggregated.log");

    /**
     *  BUILD TOPOLOGY
     *                       /---> Average -------->\
     *                      /                        \
     * source -> SenMLParse -> Kalman -> Regression -> Plot -> Sink
     *                      \                        /
     *                       \---> DistinctCt ----->/
    */

    laminar_init();

    set_host(1);
    add_host(1, "localhost", "/home/centos/laminar/");
    // add_host(2, "<put_ip_here>", "<woofc_namespace>");

    // Preferably make the Sink host the controller
    int hosts[7] = {
        1,  // SenML Parse
        1,  // Block Average
        1,  // Kalman Filter
        1,  // Linear Regression
        1,  // Distinct Count
        1,  // Plot
        1   // Sink
    };

    add_operand(1, hosts[0], 1);

    add_node(1, hosts[0], 2, {DF_CUSTOM, RIOT_SERIALIZE_SENML_PRIM});
    subscribe(1, 2, 0, 1, 1);   // SENML <--- input

    add_node(1, hosts[1], 3, {DF_CUSTOM, RIOT_AVERAGE_PRIM});
    subscribe(1, 3, 0, 1, 2);   // AVERAGE <--- SENML

    add_node(1, hosts[2], 4, {DF_CUSTOM, RIOT_KALMAN_FILTER_PRIM});
    subscribe(1, 4, 0, 1, 2);   // KALMAN <--- SENML

    add_node(1, hosts[3], 5, {DF_CUSTOM, RIOT_WINDOW_REGRESSION_PRIM});
    subscribe(1, 5, 0, 1, 4);   // REGRESSION <--- KALMAN

    add_node(1, hosts[4], 6, {DF_CUSTOM, RIOT_DISTINCT_COUNT_PRIM});
    subscribe(1, 6, 0, 1, 2);   // COUNT <--- SENML

    add_node(1, hosts[5], 7, {DF_CUSTOM, RIOT_PLOT_PRIM});
    subscribe(1, 7, 0, 1, 3);   // PLOT <--- AVERAGE
    subscribe(1, 7, 1, 1, 5);   // PLOT <--- REGRESSION
    subscribe(1, 7, 2, 1, 6);   // PLOT <--- COUNT

    add_node(1, hosts[6], 8, {DF_CUSTOM, RIOT_SINK});
    subscribe(1, 8, 0, 1, 7);   // SINK <--- PLOT

    add_operand(2, hosts[5], 1);
    add_node(2, hosts[5], 2, {DF_CUSTOM, RIOT_PLOT_SETUP});
    subscribe(2, 2, 0, 2, 1);
    laminar_setup();


    /**
     * DRIVER (SOURCE)
    */

    // only host 1 is driver
    if (get_curr_host_id() != 1) {
        return 0;
    }

    int opt, input_throughput = 0, input_duration = 0;
    // Loop through the arguments
    while ((opt = getopt(argc, argv, ARGS)) != -1) {
        switch (opt) {
        case 'm':
            input_throughput = std::atoi(optarg); // Convert option argument to int
            break;
        case 'd':
            input_duration = std::atoi(optarg); // Convert option argument to int
            break;
        case '?': // In case of unknown option
            fprintf(stderr,
                "unrecognized command %c\n",
                    (char)opt);
			fprintf(stderr,"usage: %s",Usage);
            return 1;
        }
    }

    if (input_throughput == 0)
        input_throughput = INPUT_THROUGHPUT;
    if (input_duration == 0)
        input_duration = INPUT_DURATION;

    // Install gnuplot in the CSPOT container
    struct ts_value *temp = value_from_integer(1);
    operand temp_op(temp, 1);
    fire_operand(2, 1, &temp_op);
    value_deep_delete(temp);
    sleep(5);
    get_result(2, 2, &temp_op, 1);

    std::string filename = "/home/centos/laminar/data_local/mhealth/mhealth_5_min.log";

    std::ifstream input(filename);
    std::string line;

    if (!input.is_open()) {
        std::cout << "Could not open input file" << std::endl;
        exit(1);
    }

    struct ts_value *operand_value;
    int i = 0;
    int max_messages = input_throughput * input_duration;
    int delay = 1000000 / input_throughput;

    printf("input throughput: %d messages/sec\n", input_throughput);
    printf("input duration: %d sec\n", input_duration);
    printf("max messages: %d\n", max_messages);
    printf("delay: %d usec\n", delay);
    while ((i < max_messages) && getline(input, line)) {
        operand_value = value_from_prim_string(line.c_str());
        operand op1(operand_value, i+1);
        fire_operand(1, 1, &op1);
        value_deep_delete(operand_value);
        usleep(delay);
        ++i;
    }
    printf("All messages sent!\n");

    operand return_ts;
    int ret_err = get_result(1, 8, &return_ts, i);  // wait for the last sequence to be processed
    if (ret_err < 0) {
        printf("Failed to read the result ts\n");
    }

    return 0;
}