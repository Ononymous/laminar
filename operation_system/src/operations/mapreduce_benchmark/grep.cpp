#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <cstdlib>
#include <map>
#include <chrono>
#include "mapreduce.h"

/**
 * operand[0] : string_prim (match pattern)
 * operand[1] : string_prim (data file path)
 * operand[2] : int (# of mappers)
 * operand[3] : int (# of data files)
 * 
 * output : large_string_prim (word and frequency key-value pairs)
*/
int grep_regex_mapper_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result) {
    if (operand_count < 4) {
        log_error("GREP_REGEX_MAPPER_PRIM : insufficient operands provided.");
    }
    const auto now = std::chrono::high_resolution_clock::now();
    auto start_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                             now.time_since_epoch()).count();
    std::string pattern(value_to_prim_string(operands[0]));
    std::string data_path(value_to_prim_string(operands[1]));
    int num_mappers = value_to_integer(operands[2]);
    int num_files = value_to_integer(operands[3]);

    int files_per_mapper = num_files / num_mappers;
    int file_num = (operation_metadata->node_id - 1) * files_per_mapper;

    std::unordered_map<std::string, int> matches;
    std::regex expr(pattern);

    std::string line;
    // UNCOMMENT to observe I/O throttling
    // std::vector<double> loop_times;
    for (int i = 0; i < files_per_mapper; ++i) {
        // auto loop_start_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        //                      std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        std::string file_path = data_path + "/data_" + std::to_string(file_num) + ".txt";
        ++file_num;
        std::ifstream file(file_path);
        if (!file.is_open()) {
            log_error("Could not open input file ", file_path);
            return 1;
        }

        if (!std::getline(file, line)) {
            log_error("Error reading line from file.");
            return 1;
        }

        std::istringstream iss(line);
        std::string word;

        while (std::getline(iss, word, ' ')) {
            if (std::regex_search(word, expr)) {
                if (matches.find(word) == matches.end()) {
                    matches[word] = 1;
                } else {
                    matches[word] += 1;
                }
            }
        }
        // auto loop_end_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        //                      std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        // double loop_latency_ms = (double)(loop_end_time - loop_start_time) / 1e6;
        // loop_times.push_back(loop_latency_ms);
    }

    // double temp = 0.0;
    // for(double t : loop_times)
    //     temp += t;
    // printf("Avg loop latency: %f ms\n", (temp/loop_times.size()));

    std::ostringstream oss;
    for (const auto& [key, value] : matches) {
        oss << key << " " << value << " ";
    }
    std::string mapping = oss.str();
    auto end_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    double latency_s = (double)(end_time - start_time) / 1e9;
    printf("GREP MAPPER %d COMPLETED (latency: %f s) (size: %ldB)\n", operation_metadata->node_id, latency_s, mapping.size());

    set_prim_large_string(result, mapping.c_str());

    return 1;
}

/**
 * operand[0..n] : large_string_prim (word and frequency key-value pairs)
 * 
 * output : large_string_prim (word and frequency key-value pairs)
*/
int grep_word_count_reducer_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result) {
    std::unordered_map<std::string, int> merged_counts;

    for (int op_num = 0; op_num < operand_count; op_num++) {
        char* input = value_to_prim_large_string(operands[op_num]);
        char* token = strtok(input, " ");
        while (token) {
            std::string key(token);
            if (merged_counts.find(key) == merged_counts.end()) {
                merged_counts[key] = 0;
            }

            token = std::strtok(nullptr, " ");
            merged_counts[key] += std::atoi(token);

            token = std::strtok(nullptr, " ");
        }
    }

    std::ostringstream oss;
    for (const auto& [key, value] : merged_counts) {
        oss << key << " " << value << " ";
    }
    std::string mapping = oss.str();
    printf("WC REDUCER RESULT (size: %ldB)\n", mapping.size());

    set_prim_large_string(result, mapping.c_str());

    return 1;
}

/**
 * operand[0] : large_string_prim (word and frequency key-value pairs)
 * 
 * output : large_string_prim (frequency and word key-value pairs)
*/
int grep_inverse_mapper_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result) {
    std::multimap<int, std::string, std::greater<int>> sorted_inv;

    char* input = value_to_prim_large_string(operands[0]);
    char* token = strtok(input, " ");
    while (token) {
        std::string value(token);

        token = std::strtok(nullptr, " ");
        sorted_inv.insert(make_pair(std::atoi(token), value)); // reverse key and value

        token = std::strtok(nullptr, " ");
    }

    std::ostringstream oss;
    for (const auto& [key, value] : sorted_inv) {
        oss << key << " " << value << " ";
    }
    std::string inv_mapping = oss.str();
    printf("INV MAPPER %d RESULT (size: %ldB)\n", operation_metadata->node_id, inv_mapping.size());

    set_prim_large_string(result, inv_mapping.c_str());
    return 1;
}

/**
 * operand[0..n] :  large_string_prim (frequency and word key-value pairs)
 * 
 * output : large_string_prim (frequency and word key-value pairs)
*/
int grep_frequency_reducer_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result) {
    std::multimap<int, std::string, std::greater<int>> sorted_matches;

    for (int op_num = 0; op_num < operand_count; op_num++) {
        char* input = value_to_prim_large_string(operands[op_num]);
        char* token = strtok(input, " ");
        while (token) {
            int key = std::atoi(token);

            token = std::strtok(nullptr, " ");
            std::string value(token);
            sorted_matches.insert(make_pair(key, value));

            token = std::strtok(nullptr, " ");
        }
    }

    std::ostringstream oss;
    for (const auto& [key, value] : sorted_matches) {
        oss << value << " " << key << " ";
    }
    std::string mapping = oss.str();
    printf("SORT REDUCER RESULT (size: %ldB)\n", mapping.size());

    set_prim_large_string(result, mapping.c_str());

    return 1;
}
