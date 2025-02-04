#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "riot.h"
#include "riot_config.h"

auto average(std::unordered_map<std::string, double> &input,
             std::vector<std::string> &fields,
             struct df_operation_metadata *const metadata, bool prim = false) {
    log_debug("RIOT_AVERAGE: Getting average at itr %ld", metadata->flow_control.execution_iteration);
    std::string woof_prev_avgs =
        generate_woof_path(OUT_WF_TYPE, metadata->node_namespace,
                           metadata->node_id, metadata->host_id);
    int num_prev_avgs = woof_last_seq(woof_prev_avgs);

    std::string woof_prev_inputs =
        "lmr-" + std::to_string(metadata->node_namespace) +
        ".AVG_PREV_IPS." + std::to_string(metadata->node_id);

    /* Case: First input */

    if (num_prev_avgs == 0) {
        // log_info("Case 1");
        std::unordered_map<std::string, double> result;
        for (auto &field : fields) {
            result[field] = input[field];
        }

        // Create woof to store inputs (only on first execution)
        woof_create(woof_prev_inputs, sizeof(operand), WINDOW_SIZE + 1);

        return result;
    }

    /* Case: Window has not been filled */

    // Retrieve most recent average value and deserialize
    struct operand prev_avg_op;
    const unsigned long latest_value_sequence_number = woof_last_seq(woof_prev_avgs);
    woof_get(woof_prev_avgs, &prev_avg_op, latest_value_sequence_number);
    struct ts_value *prev_avg_value = load_value(&prev_avg_op.operand_value, NULL);
    const char *prev_avg_str;
    if (prim)
        prev_avg_str = value_to_prim_string(prev_avg_value);
    else
        prev_avg_str = value_to_string(prev_avg_value);

    std::unordered_map<std::string, double> prev_avg =
        deserialize(std::string_view(prev_avg_str));

    if (num_prev_avgs < WINDOW_SIZE) {
        // log_info("Case 2");
        for (auto &field : fields) {
            double sum = prev_avg[field] * num_prev_avgs + input[field];
            prev_avg[field] = sum / (num_prev_avgs + 1);
        }

        return prev_avg;
    }

    /* Case: Window is full */

    // log_info("Case 3");
    int num_prev_inputs = woof_last_seq(woof_prev_inputs);

    // Get oldest input value within average window and deserialize
    struct operand oldest_input_op;
    woof_get(woof_prev_inputs, &oldest_input_op,
             num_prev_inputs - WINDOW_SIZE + 1);
    struct ts_value *oldest_input_value = load_value(&oldest_input_op.operand_value, NULL);
    const char *oldest_input_str;
    if (prim)
        oldest_input_str = value_to_prim_string(oldest_input_value);
    else
        oldest_input_str = value_to_string(oldest_input_value);

    std::unordered_map<std::string, double> oldest_input =
        deserialize(std::string_view(oldest_input_str));
    for (auto &field : fields) {
        prev_avg[field] += (input[field] - oldest_input[field]) / WINDOW_SIZE;
    }

    return prev_avg;
}

int average_node(const struct ts_value *const *operands,
                 const unsigned int operand_count,
                 struct df_operation_metadata *const metadata,
                 const enum ts_types result_type, struct ts_value *result) {
    log_info("AVERAGE START");

    // set window size in riot_config.h
    // int window_size = 5;
    // std::vector<std::string> fields = {
    //     "acc_chest_x", "acc_chest_y", "acc_chest_z", "ecg_lead_1",
    //     "ecg_lead_2",  "acc_ankle_x", "acc_ankle_y", "acc_ankle_z",
    //     "acc_arm_x",   "acc_arm_y",   "acc_arm_z"};

    std::vector<std::string> fields = {"test"};

    // Convert input TS_ARRAY to C-array (C-string)
    const char *input_str = value_to_string(operands[0]);

    // Deserialize data structure
    std::unordered_map<std::string, double> input =
        deserialize(std::string_view(input_str));

    // Calculate average window
    auto avg = average(input, fields, metadata);

    // Save input
    std::string woof_prev_inputs =
        "lmr-" + std::to_string(metadata->node_namespace) +
        ".AVG_PREV_IPS." + std::to_string(metadata->node_id);
    woof_put(woof_prev_inputs, "", operands[0]);

    // Serialize and return data
    std::string serialized = serialize(avg);
    set_string(result, serialized.c_str());

    return 1;
}

int average_node_prim(const struct ts_value *const *operands,
                 const unsigned int operand_count,
                 struct df_operation_metadata *const metadata,
                 const enum ts_types result_type, struct ts_value *result) {
    log_info("RIOT_AVERAGE: AVERAGE PRIM START");

    std::vector<std::string> fields = {STATS_FIELD};

    // Convert input TS_ARRAY to C-array (C-string)
    const char *input_str = value_to_prim_string(operands[0]);

    // Deserialize data structure
    std::unordered_map<std::string, double> input =
        deserialize(std::string_view(input_str));

    // Calculate average window
    auto avg = average(input, fields, metadata, true);

    // Save input
    std::string woof_prev_inputs =
        "lmr-" + std::to_string(metadata->node_namespace) +
        ".AVG_PREV_IPS." + std::to_string(metadata->node_id);
    woof_put(woof_prev_inputs, "", operands[0]);

    // Serialize and return data
    avg[TIMESTAMP_FIELD] = input[TIMESTAMP_FIELD];
    std::string serialized = serialize(avg);
    set_prim_string(result, serialized.c_str());

    return 1;
}
