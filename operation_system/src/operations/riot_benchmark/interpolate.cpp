#include "riot.h"

void interpolate(std::unordered_map<std::string, double> &current,
                 const std::string &history_woof) {
    int window_size = 5;

    if (window_size <= 0) {
        return;
    }

    // std::vector<std::string> fields = {
    //     "acc_chest_x", "acc_chest_y", "acc_chest_z", "ecg_lead_1",
    //     "ecg_lead_2",  "acc_ankle_x", "acc_ankle_y", "acc_ankle_z",
    //     "acc_arm_x",   "acc_arm_y",   "acc_arm_z"};

    std::vector<std::string> fields = {"acc_chest_x"};

    std::vector<std::unordered_map<std::string, double>> prev;

    for (std::string &field : fields) {
        // Only interpolate if value is null (and window size is valid)
        if (!current[field]) {
            int last_seq = woof_last_seq(history_woof);
            if (prev.size() == 0) {
                // Fill from history woof
                operand op;
                for (int i = last_seq; i > std::max(last_seq - window_size, 0);
                     i--) {
                    int err = woof_get(history_woof, &op, i);
                    if (err < 0) {
                        std::cout << "Failed to read the result " << std::endl;
                    }

                    ts_value *loaded_val = load_value(&op.operand_value);
                    char *result_str = static_cast<char *>(
                        malloc(get_string_length(loaded_val) + 1));
                    get_string(result_str, loaded_val);

                    prev.push_back(deserialize(result_str));
                }
            }

            // Average previous values
            double sum = 0;
            for (auto &entry : prev) {
                sum += entry[field];
            }

            // Update field with average
            current[field] = sum / prev.size();
        }
    }
}

int interpolate_node(const struct ts_value *const *operands,
                     const unsigned int operand_count,
                     struct df_operation_metadata *const operation_metadata,
                     const enum ts_types result_type, struct ts_value *result) {
    // log_info("INTERPOLATE START");

    // Convert input TS_ARRAY to C-array (C-string)
    const char* input_str = value_to_string(operands[0]);

    // Deserialize data structure
    std::unordered_map<std::string, double> input =
        deserialize(std::string_view(input_str));

    // Get woof of previous inputs
    std::string history_woof = generate_woof_path(
        OUT_WF_TYPE, operation_metadata->node_namespace,
        operation_metadata->node_id, operation_metadata->host_id);

    interpolate(input, history_woof);

    // Serialize and return data
    std::string serialized = serialize(input);
    set_string(result, serialized.c_str());

    return 1;
}

int interpolate_node_prim(const struct ts_value *const *operands,
                     const unsigned int operand_count,
                     struct df_operation_metadata *const operation_metadata,
                     const enum ts_types result_type, struct ts_value *result) {
    // log_info("INTERPOLATE PRIM START");

    // Convert input TS_ARRAY to C-array (C-string)
    const char* input_str = value_to_prim_string(operands[0]);

    // Deserialize data structure
    std::unordered_map<std::string, double> input =
        deserialize(std::string_view(input_str));

    // Get woof of previous inputs
    std::string history_woof = generate_woof_path(
        OUT_WF_TYPE, operation_metadata->node_namespace,
        operation_metadata->node_id, operation_metadata->host_id);

    interpolate(input, history_woof);

    // Serialize and return data
    std::string serialized = serialize(input);
    set_prim_string(result, serialized.c_str());

    return 1;
}
