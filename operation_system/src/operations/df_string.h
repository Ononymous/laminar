//
// Created by lukas on 11.06.2023.
//

#ifndef CSPOT_APPS_DF_STRING_H
#define CSPOT_APPS_DF_STRING_H

#include "operation_system/df_operations.h"
#include "type_system/ts_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int df_string_operation_with_type(enum df_string_ops string_operation,
                                  const struct ts_value* const* operands,
                                  unsigned int operand_count,
                                  enum ts_types result_type,
                                  struct ts_value** result);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_APPS_DF_STRING_H
