#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "riot.h"
#include "riot_config.h"

const double magic_num = 0.79402; // Durand-Flajolet magic number
const int num_buckets = 1 << BUCKET_PARAM;

int32_t* max_zeros;

/**
 * Returns ρ(y), the rank of first 1-bit from the left in y
 * 
 * return the rank of first 1-bit from the LSB
 */
int trailing_zeros(int n) {
    if (n == 0) {
        return 31;
    }

    int bits = 0;
    while ((n & 1) == 0) {
        ++bits;
        n >>= 1;
    }

    return bits;
}

/**
 * Gives the appox count of the number of distinct items that have been seen
 * this far
 *
 * Based on the Durand-Flajolet Algorithm
 * [1] Loglog Counting of Large Cardinalities, Durand and Flajolet, Sec 2
 * http://algo.inria.fr/flajolet/Publications/DuFl03-LNCS.pdf
 * [2] Mining of Massive Datasets, Jure Leskovec, et al, 2014: Sec 4.4.2 The
 * Flajolet-Martin Algorithm
 *
 * return E estimate of the cardinality of unique items in the stream, seen
 * thus far
 */
int unique_count(const char *item) {
    log_debug("UNIQUE COUNT input: %s", item);
    std::size_t hash_value = std::hash<std::string>{}(std::string(item));
    log_debug("UNIQUE COUNT hash: %ld", hash_value);

    int bucket_id = hash_value & (num_buckets - 1);

    max_zeros[bucket_id] = std::max(max_zeros[bucket_id], trailing_zeros(hash_value >> BUCKET_PARAM));

    double sum_max_zeros = 0;
    for (int i = 0; i < num_buckets; i++) {
        sum_max_zeros += max_zeros[i];
    }

    // E := α * m * 2^(1/m * Sum_j(M(j)))
    int E = (int)(magic_num * num_buckets * pow(2, sum_max_zeros / num_buckets));

    return E;
}

int distinct_count_node_internal(struct df_operation_metadata* const metadata, const char* input) {
    log_debug("DISTINCT COUNT: input: %s", input);

    // Determine number of previous outputs (is this the first iteration?)
    std::string output_woof =
        generate_woof_path(OUT_WF_TYPE, metadata->node_namespace, metadata->node_id, metadata->host_id);
    int num_outputs = woof_last_seq(output_woof);

    log_debug("DISTINCT COUNT: num of outputs: %d", num_outputs);

    std::string max_zeros_woof =
        "lmr-" + std::to_string(metadata->node_namespace) + ".MAX_ZEROS." + std::to_string(metadata->node_id);

    if (num_outputs == 0) {
        // Create woof to store max_zeros (only on first execution)
        woof_create(max_zeros_woof, sizeof(struct ts_value), 2);

        int* max_zeros_store = new int[num_buckets];
        for (int i = 0; i < num_buckets; ++i) {
            max_zeros_store[i] = 0;
        }
        ts_value* max_zeros_value = value_from_integer_array(max_zeros_store, num_buckets);
        write_value(max_zeros_value);
        woof_put(max_zeros_woof, "", max_zeros_value);
        value_deep_delete(max_zeros_value);
    }

    // Load max_zeros from woof
    struct ts_value max_zeros_value {};
    woof_get(max_zeros_woof, &max_zeros_value, woof_last_seq(max_zeros_woof));
    ts_value* max_zeros_loaded_val = load_value(&max_zeros_value, NULL);
    max_zeros = value_to_integer_array(max_zeros_loaded_val);

    int count = unique_count(input);
    log_debug("DISTINCT COUNT: got count %d", count);
    ts_value* updated_max_zeros_value = value_from_integer_array(max_zeros, num_buckets);
    write_value(updated_max_zeros_value);
    woof_put(max_zeros_woof, "", updated_max_zeros_value);
    value_deep_delete(updated_max_zeros_value);
    free(max_zeros);

    return count;
}

int distinct_count_node(const struct ts_value* const* operands,
                        const unsigned int operand_count,
                        struct df_operation_metadata* const metadata,
                        const enum ts_types result_type,
                        struct ts_value* result) {
    log_info("DISTINCT COUNT START");

    // Get input
    const char* input = value_to_string(operands[0]);

    int count = distinct_count_node_internal(metadata, input);

    set_integer(result, count);
    return 1;
}

/**
 * Do a distinct count on the entire input.
*/
int distinct_count_node_prim(const struct ts_value* const* operands,
                        const unsigned int operand_count,
                        struct df_operation_metadata* const metadata,
                        const enum ts_types result_type,
                        struct ts_value* result) {
    log_info("DISTINCT COUNT PRIM START");

    // Get input
    const char* input = value_to_prim_string(operands[0]);

    int count = distinct_count_node_internal(metadata, input);

    set_integer(result, count);
    return 1;
}