#include <chrono>

#include "riot.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

// std::string annotate(std::unordered_map<std::string, double> &m) {
//     const auto now = std::chrono::system_clock::now();

//     auto timestamp =
//         std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())
//             .count();

//     m["timestamp"] = static_cast<double>(timestamp);
// }

int annotate_node(const struct ts_value *const *operands,
                  const unsigned int operand_count,
                  const enum ts_types result_type, struct ts_value *result) {
    struct timeval tm;
    double ts;
    // log_info("ANNOTATE START");
    
    // Create timestamp for annotation
    const auto now = std::chrono::system_clock::now();
    auto timestamp =
        std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch())
            .count();

    // Convert input TS_ARRAY to C-array (C-string)
    const char *input_str = value_to_string(operands[0]);

    // Add timestamp
    std::string annotated = "timestamp~";
    annotated += std::to_string(timestamp);
    annotated += '\t';
    annotated += input_str;

    // Return result
    set_string(result, annotated.c_str());

//    gettimeofday(&tm,NULL);
//    ts = (double)(tm.tv_sec) + (double)(tm.tv_usec / 1000000.0);
//    printf("RIOT ETL STOP %f\n",ts);

    return 1;
}

int annotate_node_prim(const struct ts_value *const *operands,
                  const unsigned int operand_count,
                  const enum ts_types result_type, struct ts_value *result) {
    // log_info("ANNOTATE PRIM START");
    
    // Create timestamp for annotation
    const auto now = std::chrono::system_clock::now();
    auto timestamp =
        std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch())
            .count();

    // Convert input TS_ARRAY to C-array (C-string)
    const char *input_str = value_to_prim_string(operands[0]);

    // Add timestamp
    std::string annotated = "timestamp~";
    annotated += std::to_string(timestamp);
    annotated += '\t';
    annotated += input_str;

    // Return result
    set_prim_string(result, annotated.c_str());

    return 1;
}
