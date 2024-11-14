//
// Created by Lukas Brand on 20.03.2023.
//

#ifndef CSPOT_DF_LOGIC_H
#define CSPOT_DF_LOGIC_H

#include "operation_system/df_operations.h"
#include "type_system/ts_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
int df_logic_operation(DF_LOGIC_OP logic_operation,
                       const DF_VALUE operands[],
                       unsigned int operand_count,
                       DF_VALUE *result);
*/

int df_logic_operation_with_type(enum df_logic_ops logic_operation,
                                 const struct ts_value* const* operands,
                                 unsigned int operand_count,
                                 enum ts_types result_type,
                                 struct ts_value** result);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_DF_LOGIC_H
