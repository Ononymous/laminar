//
// Created by Lukas Brand on 17.04.2023.
//

#ifndef CSPOT_APPS_DF_LINEAR_ALGEBRA_H
#define CSPOT_APPS_DF_LINEAR_ALGEBRA_H

#include "operation_system/df_operations.h"
#include "type_system/ts_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int df_linear_algebra_operation(enum df_linear_algebra_ops linear_algebra_operation,
                                const struct ts_value* const* operands,
                                unsigned int operand_count,
                                enum ts_types result_type,
                                struct ts_value** result);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_APPS_DF_LINEAR_ALGEBRA_H
