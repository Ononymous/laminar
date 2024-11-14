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

#define INPUT_THROUGHPUT 500    // default input throughput
#define TOTAL_MESSAGES 1000   // default input duration (s)

#define ARGS "m:"
char *Usage = "msg_stress_test -m messages_per_second\n";

int main(int argc, char **argv) {
    laminar_init();

    // Set up single device on localhost
    set_host(1);
    int curr_host_id = 1;
    add_host(1, "localhost", "/home/centos/laminar/");
    add_host(2, "localhost", "/home/centos/laminar2/");

    add_operand(1, 1, 1);

    add_node(1, 1, 2, {DF_CUSTOM, RIOT_SERIALIZE_SENML_PRIM});
    subscribe(1, 2, 0, 1, 1);   // SENML <--- input

    add_node(1, 2, 3, {DF_CUSTOM, RIOT_AVERAGE_PRIM});
    subscribe(1, 3, 0, 1, 2);   // AVERAGE <--- SENML

    add_node(1, 2, 4, {DF_CUSTOM, RIOT_KALMAN_FILTER_PRIM});
    subscribe(1, 4, 0, 1, 2);   // KALMAN <--- SENML

    add_node(1, 2, 5, {DF_CUSTOM, RIOT_DISTINCT_COUNT_PRIM});
    subscribe(1, 5, 0, 1, 2);   // COUNT <--- SENML

    laminar_setup();

    if(curr_host_id == 1) {
        int opt, input_throughput = 0;
        // Loop through the arguments
        while ((opt = getopt(argc, argv, ARGS)) != -1) {
            switch (opt) {
            case 'm':
                input_throughput = std::atoi(optarg); // Convert option argument to int
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

        struct ts_value *operand_value;
        int i = 0;
        int delay = 1000000 / input_throughput;
        const char* str = "[{\"n\": \"acc_chest_x\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": -9.8184},{\"n\": \"acc_chest_y\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 0.009971},{\"n\": \"acc_chest_z\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 0.29563},{\"n\": \"ecg_1\",\"t\": \"double\",\"u\": \"mV\",\"v\": 0.0041863},{\"n\": \"ecg_2\",\"t\": \"double\",\"u\": \"mV\",\"v\": 0.0041863},{\"n\": \"acc_ankle_x\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 2.1849},{\"n\": \"acc_ankle_y\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": -9.6967},{\"n\": \"acc_ankle_z\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 0.63077},{\"n\": \"gyro_ankle_x\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": 0.1039},{\"n\": \"gyro_ankle_y\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": -0.84053},{\"n\": \"gyro_ankle_z\",\"t\": \"double\",\"u\": \"deg/s\",\"v\": -0.68762},{\"n\": \"mag_ankle_x\",\"t\": \"double\",\"u\": \"local\",\"v\": -0.37},{\"n\": \"mag_ankle_y\",\"t\": \"double\",\"u\": \"local\",\"v\": -0.36327},{\"n\": \"mag_ankle_z\",\"t\": \"double\",\"u\": \"local\",\"v\": 0.29963},{\"n\": \"acc_arm_x\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": -8.6499},{\"n\": \"acc_arm_y\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": -4.5781},{\"n\": \"acc_arm_z\",\"t\": \"double\",\"u\": \"m/s^2\",\"v\": 0.18776}]";

        while (i < TOTAL_MESSAGES) {
            operand_value = value_from_prim_string(str);
            operand op1(operand_value, i+1);
            fire_operand(1, 1, &op1);
            value_deep_delete(operand_value);
            usleep(delay);
            ++i;
        }
        printf("All messages sent!\n");
    }
    else {
        operand return_a;
        int ret_err = get_result(1, 3, &return_a, 1);  // wait for the last sequence to be processed
        if (ret_err < 0) {
            printf("Failed to read result_a\n");
        }
        const auto now1 = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now1.time_since_epoch()).count();
        operand return_b;
        ret_err = get_result(1, 3, &return_b, TOTAL_MESSAGES);  // wait for the last sequence to be processed
        if (ret_err < 0) {
            printf("Failed to read return_b\n");
        }
        const auto now2 = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now2.time_since_epoch()).count();
        double throughput = (TOTAL_MESSAGES * 1000.0) / (end - start);
        printf("All messages processed.\n");
        printf("Avg throughput: %.2f\n", throughput);
    }

    return 0;
}
