#include <chrono>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "riot.h"
#include "riot_config.h"

/**
 * Make sure every record {..., ...} starts with "n": "<name>"
 * and ends with "v": <value>.
 * 
 * TODO: Modify split to use char* or std::string instead of
 * std::string_view for generalization and position independence.
*/
std::string serialize_senml(const std::string_view& senml) {
    std::string serialized = "";
//  struct timeval tm;
//  double ts;
//    gettimeofday(&tm,NULL);
//    ts = (double)(tm.tv_sec) + (double)(tm.tv_usec / 1000000.0);
//   printf("RIOT ETL START %f\n",ts);
    const auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                            now.time_since_epoch()).count();

    // Remove cruft
    std::string_view senml_clean = senml.substr(2, senml.size() - 4);

    // Extract values
    std::vector<std::string_view> entries = split(senml_clean, "},{");
    for (auto& e : entries) {
        std::vector<std::string_view> kv_pairs = split(e, ",");

        // Key
        std::string_view key_pair = kv_pairs[0];
        key_pair.remove_suffix(1);
        std::string_view key = key_pair.substr(kv_pairs[0].find(' ') + 2);
        serialized += key;
        serialized += '~';

        // Value
        std::string_view value_pair = kv_pairs.back();
        std::string_view value = value_pair.substr(value_pair.find(' ') + 1);
        serialized += value;
        serialized += '\t';
    }

    serialized += std::string(TIMESTAMP_FIELD) + "~" + std::to_string(timestamp);
    return serialized;
}

int serialize_senml_node(const struct ts_value* const* operands,
                         const unsigned int operand_count,
                         const enum ts_types result_type,
                         struct ts_value* result) {
    // log_info("SERIALIZE SENML START");

    // Convert input TS_ARRAY to C-array (C-string)
    const char* string = value_to_string(operands[0]);

    std::string serialized = serialize_senml(std::string_view(string));

    // Convert result from C-array to TS_ARRAY to be returned
    set_string(result, serialized.c_str());

    return 1;
}

int serialize_senml_node_prim(const struct ts_value* const* operands,
                         const unsigned int operand_count,
                         struct df_operation_metadata *const metadata,
                         const enum ts_types result_type,
                         struct ts_value* result) {
    log_info("SENML: SERIALIZE SENML PRIM START");

    // Convert input TS_ARRAY to C-array (C-string)
    const char* string = value_to_prim_string(operands[0]);

    std::string serialized = serialize_senml(std::string_view(string));

    // Convert result from C-array to TS_ARRAY to be returned
    set_prim_string(result, serialized.c_str());

    return 1;
}
