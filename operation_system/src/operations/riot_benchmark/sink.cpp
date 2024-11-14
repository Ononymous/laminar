#include <chrono>
#include <fstream>
#include <iostream>

#include "riot.h"
#include "riot_config.h"

int sink_node(const struct ts_value *const *operands,
        const unsigned int operand_count,
        struct df_operation_metadata *const metadata,
        const enum ts_types result_type, struct ts_value *result) {
    const char *input_str = value_to_prim_string(operands[0]);
    std::unordered_map<std::string, double> input =
        deserialize(std::string_view(input_str));

    std::string file_path = std::string(DATA_PATH) + "riot_sink.csv";
    std::ofstream file(file_path, std::ios_base::app);
    if (!file.is_open()) {
        log_error("Could not open output file ", file_path);
        return 1;
    }

    const auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now.time_since_epoch()).count();

    file << std::to_string(metadata->flow_control.execution_iteration)
            << ", " << std::to_string((unsigned long)input[TIMESTAMP_FIELD])
            << ", " << std::to_string(timestamp) << "\n";

    set_long(result, timestamp);
    return 1;
}