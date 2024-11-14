#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "riot.h"
#include "riot_config.h"

using SDMap = std::unordered_map<std::string, double>;

/**
 * p0: prior error covariance
 * p1: current error covariance
 * x0: previous estimation
 * x1: current estimation
 */

float q_processNoise = 0.125;
float r_sensorNoise = 0.32;

float kalman_filter(float z_measuredValue, double &p0, double &x0) {
    // Time update
    float p1 = p0 + q_processNoise;

    // Measurement update
    float k_kalmanGain = p1 / (p1 + r_sensorNoise);
    float x1 = x0 + k_kalmanGain * (z_measuredValue - x0);
    p1 = (1 - k_kalmanGain) * p1;

    // Update values
    x0 = x1;
    p0 = p1;

    return x1;
}

template <size_t N>
SDMap kalman_filter(SDMap &input, SDMap &p0s, SDMap &x0s,
                    const std::array<const char *, N> &fields) {
    SDMap estimates;

    // Get estimates, update p0s and x0s
    for (auto &field : fields) {
        float estimate = kalman_filter(input[field], p0s[field], x0s[field]);
        estimates[field] = estimate;
    }

    return estimates;
}

int kalman_filter_node(const struct ts_value *const *operands,
                       const unsigned int operand_count,
                       struct df_operation_metadata *const metadata,
                       const enum ts_types result_type,
                       struct ts_value *result) {
    log_info("KALMAN FILTER START");

    //   constexpr const std::array fields = {
    //       "acc_chest_x", "acc_chest_y", "acc_chest_z", "ecg_lead_1",
    //       "ecg_lead_2",  "acc_ankle_x", "acc_ankle_y", "acc_ankle_z",
    //       "acc_arm_x",   "acc_arm_y",   "acc_arm_z",
    //   };

    constexpr const std::array fields = {"test"};

    // Deserialize input
    const char *input_str = value_to_string(operands[0]);
    SDMap input = deserialize(std::string_view(input_str));

    // Determine number of previous outputs (is this the first iteration?)
    std::string output_woof =
        generate_woof_path(OUT_WF_TYPE, metadata->node_namespace,
                           metadata->node_id, metadata->host_id);
    int num_outputs = woof_last_seq(output_woof);

    std::string p0s_woof = "lmr-" +
                           std::to_string(metadata->node_namespace) +
                           ".p0_VALUES." + std::to_string(metadata->node_id);
    std::string x0s_woof = "lmr-" +
                           std::to_string(metadata->node_namespace) +
                           ".x0_VALUES." + std::to_string(metadata->node_id);

    // Fetch p0s and x0s
    SDMap p0s, x0s;
    if (num_outputs == 0) {
        // Create woofs to store p0s and x0s (only on first execution)
        woof_create(p0s_woof, sizeof(ts_value), 2);
        woof_create(x0s_woof, sizeof(ts_value), 2);

        for (auto &field : fields) {
            p0s[field] = 30.0;
            x0s[field] = 0.0;
        }
    } else {
        // Load p0s from woof
        struct ts_value p0s_value;
        woof_get(p0s_woof, &p0s_value, 0);
        ts_value* p0s_loaded_val = load_value(&p0s_value);
        const char *p0s_str = value_to_string(p0s_loaded_val);
        p0s = deserialize(std::string_view(p0s_str));

        // Load x0s from woof
        struct ts_value x0s_value;
        woof_get(x0s_woof, &x0s_value, 0);
        ts_value* x0s_loaded_val = load_value(&x0s_value);
        const char *x0s_str = value_to_string(x0s_loaded_val);
        x0s = deserialize(std::string_view(x0s_str));
    }

    // Calculate Kalman filter estimates, update p0s and x0s
    auto estimates = kalman_filter(input, p0s, x0s, fields);

    // Save p0s and x0s
    struct ts_value value;
    std::string serialized = serialize(p0s);
    set_string(&value, serialized.c_str());
    write_value(&value);
    woof_put(p0s_woof, "", &value);
    
    serialized = serialize(x0s);
    set_string(&value, serialized.c_str());
    write_value(&value);
    woof_put(x0s_woof, "", &value);

    // Serialize and return data
    serialized = serialize(estimates);
    set_string(result, serialized.c_str());

    return 1;
}

int kalman_filter_node_prim(const struct ts_value *const *operands,
                       const unsigned int operand_count,
                       struct df_operation_metadata *const metadata,
                       const enum ts_types result_type,
                       struct ts_value *result) {
    log_info("KALMAN FILTER PRIM START");

    constexpr const std::array fields = {STATS_FIELD};

    // Deserialize input
    const char *input_str = value_to_prim_string(operands[0]);
    SDMap input = deserialize(std::string_view(input_str));

    // Determine number of previous outputs (is this the first iteration?)
    std::string output_woof =
        generate_woof_path(OUT_WF_TYPE, metadata->node_namespace,
                           metadata->node_id, metadata->host_id);
    int num_outputs = woof_last_seq(output_woof);

    std::string p0s_woof = "lmr-" +
                           std::to_string(metadata->node_namespace) +
                           ".p0_VALUES." + std::to_string(metadata->node_id);
    std::string x0s_woof = "lmr-" +
                           std::to_string(metadata->node_namespace) +
                           ".x0_VALUES." + std::to_string(metadata->node_id);

    // Fetch p0s and x0s
    SDMap p0s, x0s;
    if (num_outputs == 0) {
        // Create woofs to store p0s and x0s (only on first execution)
        woof_create(p0s_woof, sizeof(ts_value), 2);
        woof_create(x0s_woof, sizeof(ts_value), 2);

        for (auto &field : fields) {
            p0s[field] = 30.0;
            x0s[field] = 0.0;
        }
    } else {
        // Load p0s from woof
        struct ts_value p0s_value;
        woof_get(p0s_woof, &p0s_value, 0);
        ts_value* p0s_loaded_val = load_value(&p0s_value);
        const char *p0s_str = value_to_string(p0s_loaded_val);
        p0s = deserialize(std::string_view(p0s_str));

        // Load x0s from woof
        struct ts_value x0s_value;
        woof_get(x0s_woof, &x0s_value, 0);
        ts_value* x0s_loaded_val = load_value(&x0s_value);
        const char *x0s_str = value_to_string(x0s_loaded_val);
        x0s = deserialize(std::string_view(x0s_str));
    }

    // Calculate Kalman filter estimates, update p0s and x0s
    auto estimates = kalman_filter(input, p0s, x0s, fields);

    // Save p0s and x0s
    struct ts_value value;
    std::string serialized = serialize(p0s);
    set_string(&value, serialized.c_str());
    write_value(&value);
    woof_put(p0s_woof, "", &value);
    
    serialized = serialize(x0s);
    set_string(&value, serialized.c_str());
    write_value(&value);
    woof_put(x0s_woof, "", &value);

    // Serialize and return data
    serialized = serialize(estimates);
    set_prim_string(result, serialized.c_str());

    return 1;
}