#ifndef KS_TEST_H
#define KS_TEST_H

#ifdef ESP8266
#include "df_interface.h"
#include "ts_type.h"
#else
#include "../../../../df_interface.h"
#include "type_system/ts_type.h"
#endif

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>


/////////////////
// KS Benchmark
/////////////////


int ks_test_node(const struct ts_value *const *operands,
                     const unsigned int operand_count,
                     struct df_operation_metadata *const metadata,
                     const enum ts_types result_type,
                     struct ts_value *result);
#endif
