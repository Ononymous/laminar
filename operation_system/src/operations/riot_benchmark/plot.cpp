#include <algorithm>
#include <cstdlib>
#include <sciplot/sciplot.hpp>
#include <vector>

#include "riot.h"
#include "riot_config.h"

using namespace sciplot;

void plot_line(double m, double b, const std::string& filename) {
    auto f = [&m, &b](double x) { return m * x + b; };

    // Create values for your x-axis
    std::vector<double> x;
    for (int i = 0; i <= 100; i += 5) {
        x.push_back(i);
    }

    std::vector<double> y = x;
    std::for_each(y.begin(), y.end(), f);

    // Create a Plot object
    Plot2D plot1;
    // Set color palette for first Plot
    plot1.palette("paired");
    
    plot1.drawCurve(x, y).label("regression").lineWidth(2);

    Figure figure = {{plot1}};

    Canvas canvas = {{figure}};
    // Set color palette for all Plots that do not have a palette set (plot2) / the default palette
    canvas.defaultPalette("set1");

    // Save the plot to a file
    canvas.save(filename);
}

int plot_node(const struct ts_value* const* operands,
                      const unsigned int operand_count,
                      struct df_operation_metadata* const operation_metadata,
                      const enum ts_types result_type,
                      struct ts_value* const result) {
    log_info("PLOT START");

    // Convert input TS_ARRAY to C-array
    const double* m_and_b = value_to_double_array(operands[0]);
    double m = m_and_b[0], b = m_and_b[1];

    // Determine whether or not value should be filtered
    // operation_metadata->flow_control.write_value = !filter;

    // Generate plot
    plot_line(m, b, "test.svg");

    // Zip plot file
    // std::system("zip test.zip test.svg");

    return 1;
}


void generate_multiline_plot(std::unordered_map<std::string, double> &curr_tuple, struct df_operation_metadata* const metadata) {
    std::vector<double> series1{curr_tuple["avg"]};
    std::vector<double> series2{curr_tuple["pred"]};
    std::vector<double> series3{curr_tuple["count"]};
    std::vector<double> x_axis;

    std::string woof_prev_obs =
        "lmr-" + std::to_string(metadata->node_namespace) +
        ".PLOT_PREV_OBS." + std::to_string(metadata->node_id);
    int end_seq = woof_last_seq(woof_prev_obs);
    x_axis.push_back(end_seq + 1);

    struct operand old_obs_op;
    struct ts_value *old_obs_value;
    for (int i = 0; i < PLOT_ACCUMULATE_SIZE - 1; ++i) {
        woof_get(woof_prev_obs, &old_obs_op, end_seq - i);
        old_obs_value = load_value(&old_obs_op.operand_value, NULL);
        const char *old_obs_str = value_to_prim_string(old_obs_value);
        std::unordered_map<std::string, double> old_obs =
            deserialize(std::string_view(old_obs_str));
        series1.insert(series1.begin(), old_obs["avg"]);
        series2.insert(series2.begin(), old_obs["pred"]);
        series3.insert(series3.begin(), old_obs["count"]);
        value_deep_delete(old_obs_value);
    }

    sciplot::Plot2D plot;
    plot.palette("paired");

    plot.drawCurve(x_axis, series1).label("Block Average");
    plot.drawCurve(x_axis, series2).label("Next Prediction");
    plot.drawCurve(x_axis, series3).label("Distinct Count");

    Figure fig = {{plot}};

    Canvas canvas = {{fig}};
    canvas.defaultPalette("set1");

    log_debug("PLOT: Generated plot.");
    std::string filename = std::string(DATA_PATH) + "plot.png";
    canvas.save(filename);
}

/**
* The 3 inputs to plot:
* 1. Windowed average of 1 field.
* 2. Regression prediction of 1 field.
* 3. Distinct count for field.
*/
int plot_node_prim(const struct ts_value* const* operands,
                const unsigned int operand_count,
                struct df_operation_metadata* const metadata,
                const enum ts_types result_type,
                struct ts_value* const result) {
    log_info("PLOT: PLOT START");

    std::string field = STATS_FIELD;

    // Get the average node output
    const char *input_str = value_to_prim_string(operands[0]);
    std::unordered_map<std::string, double> average =
        deserialize(std::string_view(input_str));

    // Get the window regression prediction
    input_str = value_to_prim_string(operands[1]);
    std::unordered_map<std::string, double> prediction =
        deserialize(std::string_view(input_str));

    // Get the distinct count
    int distinct_count = operands[2]->value.ts_int;

    std::unordered_map<std::string, double> tuple;
    tuple["avg"] = average[field];
    tuple[TIMESTAMP_FIELD] = average[TIMESTAMP_FIELD];
    tuple["pred"] = prediction[field];
    tuple["count"] = distinct_count;
    std::string tuple_str = serialize(tuple);
    set_prim_string(result, tuple_str.c_str()); // send accumulated information to the sink

    // Write to file to simulate blob upload
    std::string file_path = std::string(DATA_PATH) + "riot_aggregated.log";
    std::ofstream file(file_path, std::ios_base::app);
    if (!file.is_open()) {
        log_error("Could not open output file ", file_path);
        return 1;
    }
    file << tuple_str << "\n";

    // OUTPUT WooF for the node
    std::string woof_prev_outputs =
        generate_woof_path(OUT_WF_TYPE, metadata->node_namespace,
                           metadata->node_id, metadata->host_id);
    unsigned long num_prev_obs = woof_last_seq(woof_prev_outputs);

    //  Observation history WooF
    std::string woof_prev_obs =
        "lmr-" + std::to_string(metadata->node_namespace) +
        ".PLOT_PREV_OBS." + std::to_string(metadata->node_id);

    if (num_prev_obs == 0) {
        woof_create(woof_prev_obs, sizeof(operand), PLOT_ACCUMULATE_SIZE + 1);
        return 1;
    }

    // Got a set of PLOT_ACCUMULATE_SIZE inputs
    if ((num_prev_obs + 1) % PLOT_ACCUMULATE_SIZE == 0) {
        log_info("PLOT: Generating a graph at: %ld elements", num_prev_obs+1);
        generate_multiline_plot(tuple, metadata);
    }

    // Add the latest aggregate to the history woof
    struct ts_value *latest = value_from_prim_string(tuple_str.c_str());
    woof_put(woof_prev_obs, "", latest);
    return 1;
}

int setup_plot_node(const struct ts_value* const* operands,
                const unsigned int operand_count,
                struct df_operation_metadata* const metadata,
                const enum ts_types result_type,
                struct ts_value* const result) {
    int status = std::system("yum -y install gnuplot");
    set_integer(result, 1);
    return 1;
}