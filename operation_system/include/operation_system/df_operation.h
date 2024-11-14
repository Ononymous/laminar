//
// Created by Lukas Brand on 09.01.2023.
//
#ifndef CSPOT_DF_OPERATION_H
#define CSPOT_DF_OPERATION_H

#ifdef ESP8266
#include "ts_types.h"
#else
#include "type_system/ts_types.h"
#endif
#include "df_operations.h"

#ifdef __cplusplus
extern "C" {
#endif

bool df_operation_with_type(struct df_operation operation,
                           const struct ts_value* const* operands,
                           size_t operand_count,
                           struct df_operation_metadata* operation_metadata,
                            enum ts_types result_type,
                           struct ts_value** result);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_DF_OPERATION_H
