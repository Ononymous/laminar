#ifndef RIOT_H
#define RIOT_H

#include "../../../../df_interface.h"
#include "logger_system/df_logger.h"
#include "logger_system/df_operation_logger.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

/////////////////
// RIoT Benchmark
/////////////////

/* ETL */

int serialize_senml_node(const struct ts_value *const *operands,
                         const unsigned int operand_count,
                         const enum ts_types result_type,
                         struct ts_value *result);

int serialize_senml_node_prim(const struct ts_value *const *operands,
                         const unsigned int operand_count,
                         struct df_operation_metadata *const metadata,
                         const enum ts_types result_type,
                         struct ts_value *result);

int range_filter_node(const struct ts_value *const *operands,
                      const unsigned int operand_count,
                      struct df_operation_metadata *const metadata,
                      const enum ts_types result_type,
                      struct ts_value *const result);

int range_filter_node_prim(const struct ts_value *const *operands,
                      const unsigned int operand_count,
                      struct df_operation_metadata *const metadata,
                      const enum ts_types result_type,
                      struct ts_value *const result);

int bloom_filter_node(const struct ts_value *const *operands,
                      const unsigned int operand_count,
                      struct df_operation_metadata *const metadata,
                      const enum ts_types result_type,
                      struct ts_value *const result);

int bloom_filter_node_prim(const struct ts_value *const *operands,
                      const unsigned int operand_count,
                      struct df_operation_metadata *const metadata,
                      const enum ts_types result_type,
                      struct ts_value *const result);

int interpolate_node(const struct ts_value *const *operands,
                     const unsigned int operand_count,
                     struct df_operation_metadata *const metadata,
                     const enum ts_types result_type,
                     struct ts_value *result);

int interpolate_node_prim(const struct ts_value *const *operands,
                     const unsigned int operand_count,
                     struct df_operation_metadata *const metadata,
                     const enum ts_types result_type,
                     struct ts_value *result);

int annotate_node(const struct ts_value *const *operands,
                  const unsigned int operand_count,
                  const enum ts_types result_type,
                  struct ts_value *result);

int annotate_node_prim(const struct ts_value *const *operands,
                  const unsigned int operand_count,
                  const enum ts_types result_type,
                  struct ts_value *result);

/* STATS */

int average_node(const struct ts_value *const *operands,
                 const unsigned int operand_count,
                 struct df_operation_metadata *const metadata,
                 const enum ts_types result_type,
                 struct ts_value *result);

int average_node_prim(const struct ts_value *const *operands,
                 const unsigned int operand_count,
                 struct df_operation_metadata *const metadata,
                 const enum ts_types result_type,
                 struct ts_value *result);

int kalman_filter_node(const struct ts_value *const *operands,
                       const unsigned int operand_count,
                       struct df_operation_metadata *const metadata,
                       const enum ts_types result_type,
                       struct ts_value *result);

int kalman_filter_node_prim(const struct ts_value *const *operands,
                       const unsigned int operand_count,
                       struct df_operation_metadata *const metadata,
                       const enum ts_types result_type,
                       struct ts_value *result);

int distinct_count_node(const struct ts_value *const *operands,
                        const unsigned int operand_count,
                        struct df_operation_metadata *const metadata,
                        const enum ts_types result_type,
                        struct ts_value *result);

int distinct_count_node_prim(const struct ts_value *const *operands,
                        const unsigned int operand_count,
                        struct df_operation_metadata *const metadata,
                        const enum ts_types result_type,
                        struct ts_value *result);

int regression_node(const struct ts_value *const *operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata *const metadata,
                    const enum ts_types result_type,
                    struct ts_value *result);

int window_regression_node_prim(const struct ts_value *const *operands,
                    const unsigned int operand_count,
                    struct df_operation_metadata *const metadata,
                    const enum ts_types result_type,
                    struct ts_value *result);

int plot_node(const struct ts_value *const *operands,
              const unsigned int operand_count,
              struct df_operation_metadata *const metadata,
              const enum ts_types result_type,
              struct ts_value *result);

int plot_node_prim(const struct ts_value *const *operands,
              const unsigned int operand_count,
              struct df_operation_metadata *const metadata,
              const enum ts_types result_type,
              struct ts_value *result);

int setup_plot_node(const struct ts_value* const* operands,
                const unsigned int operand_count,
                struct df_operation_metadata* const metadata,
                const enum ts_types result_type,
                struct ts_value* const result);

int sink_node(const struct ts_value *const *operands,
        const unsigned int operand_count,
        struct df_operation_metadata *const metadata,
        const enum ts_types result_type,
        struct ts_value *result);


/* Common */

std::vector<std::string_view> split(const std::string_view &s,
                                    const std::string &sep);

void split(const std::string_view &s, const std::string &sep,
           std::function<void(std::string_view &)> &f);

std::string serialize(const std::unordered_map<std::string, double> &m);
std::unordered_map<std::string, double> deserialize(const std::string_view &s);

#endif
