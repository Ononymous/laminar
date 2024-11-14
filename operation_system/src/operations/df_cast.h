//
// Created by Lukas Brand on 11.04.2023.
//

#ifndef CSPOT_DF_CAST_H
#define CSPOT_DF_CAST_H

#include "operation_system/df_operations.h"
#include "type_system/ts_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int df_cast_operation(enum df_cast_ops cast_operation,
                      const struct ts_value* const* operands,
                      unsigned int operand_count,
                      struct ts_value** result);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_DF_CAST_H
