//
// Created by Lukas Brand on 20.03.2023.
//

#ifndef CSPOT_DF_ARITHMETIC_H
#define CSPOT_DF_ARITHMETIC_H

#include "operation_system/df_operations.h"
#include "type_system/ts_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int df_arithmetic_operation_with_type(enum df_arithmetic_ops arithmetic_operation,
                                      const struct ts_value* const* operands,
                                      unsigned int operand_count,
                                      enum ts_types result_type,
                                      struct ts_value** result);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_DF_ARITHMETIC_H
