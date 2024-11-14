//
// Created by Lukas Brand on 20.03.2023.
//

#ifndef CSPOT_DF_CUSTOM_OPERATIONS_H
#define CSPOT_DF_CUSTOM_OPERATIONS_H

#ifdef ESP8266
#include "df_operations.h"
#include "ts_types.h"
#else
#include "operation_system/df_operations.h"
#include "type_system/ts_types.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int df_custom_operation(enum df_custom_ops custom_operation,
                        const struct ts_value* const* operands,
                        unsigned int operand_count,
                        struct df_operation_metadata* operation_metadata,
                        enum ts_types result_type,
                        struct ts_value** result);

#ifdef __cplusplus
}
#endif
#endif // CSPOT_DF_CUSTOM_OPERATIONS_H
