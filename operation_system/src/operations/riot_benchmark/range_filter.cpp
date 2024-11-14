#include "riot.h"

struct range {
    range(double min_val, double max_val) : min(min_val), max(max_val) {}
    double min;
    double max;
};

bool range_filter(std::unordered_map<std::string, double>& input) {
    std::unordered_map<std::string, range> ranges = {
        {"acc_chest_x", {-13.931, 4.123}},  {"acc_chest_y", {-4.6376, 5.2361}},
        {"acc_chest_z", {-8.1881, 7.8786}}, {"ecg_lead_1", {-4.9314, 6.1371}},
        {"ecg_lead_2", {-6.786, 6.6604}},   {"acc_ankle_x", {-5.0006, 8.1472}},
        {"acc_ankle_y", {-14.303, 1.5909}}, {"acc_ankle_z", {-8.6234, 8.6958}},
        {"acc_arm_x", {-9.824, 5.5778}},    {"acc_arm_y", {-10.059, 8.506}},
        {"acc_arm_z", {-6.6739, 9.5725}}};

    for (auto& [key, valid_range] : ranges) {
        double val = input[key];
        if (val < valid_range.min || val > valid_range.max) {
            return true;
        }
    }

    return false;
}

int range_filter_node(const struct ts_value* const* operands,
                      const unsigned int operand_count,
                      struct df_operation_metadata* const operation_metadata,
                      const enum ts_types result_type,
                      struct ts_value* const result) {
    // log_info("RANGE FILTER START");

    // Convert input TS_ARRAY to C-array (C-string)
    const char* input_str = value_to_string(operands[0]);

    // Deserialize data structure
    std::unordered_map<std::string, double> input =
        deserialize(std::string_view(input_str));

    // Determine whether or not value should be filtered
    bool filter = range_filter(input);
    operation_metadata->flow_control.write_value = !filter;

    // Serialize and return data
    std::string serialized = serialize(input);
    set_string(result, serialized.c_str());

    return 1;
}

int range_filter_node_prim(const struct ts_value* const* operands,
                      const unsigned int operand_count,
                      struct df_operation_metadata* const operation_metadata,
                      const enum ts_types result_type,
                      struct ts_value* const result) {
    // log_info("RANGE FILTER PRIM START");

    // Convert input TS_ARRAY to C-array (C-string)
    const char* input_str = value_to_prim_string(operands[0]);

    // Deserialize data structure
    std::unordered_map<std::string, double> input =
        deserialize(std::string_view(input_str));

    // Determine whether or not value should be filtered
    bool filter = range_filter(input);
    operation_metadata->flow_control.write_value = !filter;

    // Serialize and return data
    std::string serialized = serialize(input);
    set_prim_string(result, serialized.c_str());

    return 1;
}
