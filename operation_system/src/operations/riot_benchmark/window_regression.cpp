#include <cmath>
#include <random>

#include "riot.h"
#include "riot_config.h"

std::pair<double, double> simple_regression(int itemCount, double value, struct df_operation_metadata *const metadata) {
    std::vector<double> x;
    x.push_back(itemCount);

    std::vector<double> y;
    y.push_back(value);

    std::string woof_prev_inputs =
        "lmr-" + std::to_string(metadata->node_namespace) +
        ".REGR_PREV_IPS." + std::to_string(metadata->node_id);
    int end_seq = woof_last_seq(woof_prev_inputs);

    // First training point was the current itemCount and value.
    // Fetch the remaining points from history woof.
    struct operand old_input_op;
    struct ts_value *old_input_value;
    for (int i = 0; i < WINDOW_SIZE - 1; ++i) {
        woof_get(woof_prev_inputs, &old_input_op, end_seq - i);
        old_input_value = load_value(&old_input_op.operand_value);
        const char *old_input_str = value_to_prim_string(old_input_value);
        std::unordered_map<std::string, double> old_input =
            deserialize(std::string_view(old_input_str));
        x.push_back(itemCount - i - 1);
        y.push_back((*old_input.begin()).second);
        value_deep_delete(old_input_value);
    }
    
    double sum_x = 0, sum_y = 0;
    double avg_x = 0, avg_y = 0;
    int num_items = x.size();

    for (double t: x)
        sum_x += t;
    
    for (double t: y)
        sum_y += t;

    avg_x = sum_x / num_items;
    avg_y = sum_y / num_items;

    double numerator = 0, denominator = 0, x_err = 0;
    for (int i = 0; i < num_items; ++i) {
        x_err = x[i] - avg_x;
        numerator += x_err * (y[i] - avg_y);
        denominator += pow(x_err, 2);
    }

    double alpha = numerator / denominator;
    double beta = avg_y - (alpha * avg_x);

    return std::make_pair(alpha, beta);
}

/**
 * For regression, the count of the current input is the independent variable,
 * and 1 message field is the dependent variable.
 * Follow the structure laid out in:
 * https://github.com/dream-lab/riot-bench/blob/master/modules/tasks/src/main/java/in/dream_lab/bm/stream_iot/tasks/predict/SimpleLinearRegressionPredictor.java
*/
int window_regression_node_prim(const struct ts_value *const *operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata *const metadata,
                    const enum ts_types result_type, struct ts_value *result) {
    log_info("REGRESSION PRIM START");

    std::string msg_field = STATS_FIELD;  // the field on which regression is being done

    const char *input_str = value_to_prim_string(operands[0]);
    log_debug("REGRESSION: Input string: %s", input_str);
    std::unordered_map<std::string, double> input =
        deserialize(std::string_view(input_str));

    double curr_val = input[msg_field]; // current data point is (num_prev_inputs+1, curr_val)
    double prediction = curr_val;  // the final prediction from the regression line, set to the current input by default

    std::string woof_prev_preds =
        generate_woof_path(OUT_WF_TYPE, metadata->node_namespace,
                           metadata->node_id, metadata->host_id);
    int itemCount = woof_last_seq(woof_prev_preds) + 1;  // message number of the current input
    log_debug("REGRESSION: Number of previous predictions: %d", itemCount - 1);

    std::string woof_prev_inputs =
        "lmr-" + std::to_string(metadata->node_namespace) +
        ".REGR_PREV_IPS." + std::to_string(metadata->node_id);

    if (itemCount == 1) {
        // First value: create the history woof
        woof_create(woof_prev_inputs, sizeof(operand), WINDOW_SIZE + 1);
    }
    else if (itemCount >= WINDOW_SIZE) {
        std::pair<double, double> params = simple_regression(itemCount, curr_val, metadata);
        log_debug("REGRESSION: got the params:: slope %f, intercept %f", params.first, params.second);
        prediction = ((itemCount + 1) * params.first) + params.second; // predict the next value
    }

    // Only save the message field for current input
    woof_put(woof_prev_inputs, "", operands[0]);

    // Serialize and return data
    input[msg_field] = prediction;
    std::string serialized = serialize(input);
    set_prim_string(result, serialized.c_str());

    return 1;
}