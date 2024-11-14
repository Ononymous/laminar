//
// Created by Lukas Brand on 20.03.2023.
//

#ifndef CSPOT_DF_INTERNAL_H
#define CSPOT_DF_INTERNAL_H

#include "operation_system/df_operations.h"
#include "type_system/ts_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
int df_internal_operation(DF_INTERNAL_OP internal_operation,
                          const DF_VALUE operands[],
                          unsigned int operand_count,
                          DF_VALUE* result);
*/

int df_internal_operation_with_type(enum df_internal_ops internal_operation,
                                    const struct ts_value* const* operands,
                                    unsigned int operand_count,
                                    struct df_operation_metadata* operation_metadata,
                                    enum ts_types result_type,
                                    struct ts_value** result);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_DF_INTERNAL_H
