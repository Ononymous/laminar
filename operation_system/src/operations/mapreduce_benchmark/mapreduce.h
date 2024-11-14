#ifndef MAPREDUCE_H
#define MAPREDUCE_H

#include "../../../../df_interface.h"
#include "logger_system/df_logger.h"
#include "logger_system/df_operation_logger.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_primitive.h"

#include <fstream>
#include <string>
#include <unordered_map>

int k_means_clustering_mapper_node_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result);

int k_means_clustering_reducer_node_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result);

int logistic_regression_mapper_node_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result);

int logistic_regression_reducer_node_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result);

int grep_regex_mapper_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result);

int grep_word_count_reducer_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result);

int grep_inverse_mapper_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result);

int grep_frequency_reducer_prim(const struct ts_value* const* operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata* const operation_metadata,
                    const enum ts_types result_type,
                    struct ts_value* const result);

#endif
