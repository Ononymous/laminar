#include <cmath>
#include <random>

#include "riot.h"

struct regression_data {
    double num;
    double x;
    double y;
    double xx;
    double xy;

    regression_data() : num(0.0), x(0.0), y(0.0), xx(0.0), xy(0.0) {}
};

std::pair<double, double> regression(double new_x, double new_y,
                                     regression_data &r) {
    double dt = 1e-2;  // Time step (delta t)
    double T = 5e-2;   // Time constant
    double decay_factor = exp(-dt / T);
    double slope, intercept;

    // Decay values
    r.num *= decay_factor;
    r.x *= decay_factor;
    r.y *= decay_factor;
    r.xx *= decay_factor;
    r.xy *= decay_factor;

    // Add new datapoint
    r.num += 1;
    r.x += new_x;
    r.y += new_y;
    r.xx += new_x * new_x;
    r.xy += new_x * new_y;

    // Calculate determinant and new slope / intercept
    double det = r.num * r.xx - pow(r.x, 2);
    if (det > 1e-10) {
        intercept = (r.xx * r.y - r.xy * r.x) / det;
        slope = (r.xy * r.num - r.x * r.y) / det;
    }

    return {slope, intercept};
}

double noise() {
    double random_num = (double)(rand()) / RAND_MAX;  // [0, 1]
    return (random_num - 0.5) * 0.1;                  // [-0.1, 0.1]
}

int regression_node(const struct ts_value *const *operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata *const metadata,
                    const enum ts_types result_type, struct ts_value *result) {
    log_info("REGRESSION START");

    // Create woof to store inputs (only on first execution)
    std::string output_woof =
        generate_woof_path(OUT_WF_TYPE, metadata->node_namespace,
                           metadata->node_id, metadata->host_id);
    std::string regression_data_woof =
        "lmr-" + std::to_string(metadata->node_namespace) +
        ".REGRESSION_DATA." + std::to_string(metadata->node_id);

    int num_outputs = woof_last_seq(output_woof);
    if (num_outputs == 0) {
        woof_create(regression_data_woof, sizeof(regression_data), 2);
    }

    // Retrieve regression variables
    regression_data r;
    woof_get(regression_data_woof, &r, 0);

    auto y = [](double x) { return 2 * x + 3; };

    double x = rand() % 100;

    // Perform regression
    auto [slope, intercept] = regression(x, y(x) + noise(), r);

    // Save regression data
    woof_put(regression_data_woof, "", &r);

    // Serialize and return result
    double line[] = {slope, intercept};
    set_double_array(result, line, 2);

    return 1;
}
