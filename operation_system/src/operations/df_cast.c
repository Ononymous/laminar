//
// Created by Lukas Brand on 11.04.2023.
//
#include "df_cast.h"

int df_cast_operation(enum df_cast_ops cast_operation,
                      const struct ts_value* const* operands,
                      unsigned int operand_count,
                      struct ts_value** const result) {
    switch (cast_operation) {
        case DF_CAST_TO_BOOLEAN:
            break;
        case DF_CAST_TO_BYTE:
            break;
        case DF_CAST_TO_SHORT:
            break;
        case DF_CAST_TO_INTEGER:
            break;
        case DF_CAST_TO_LONG:
            break;
        case DF_CAST_TO_UNSIGNED_BYTE:
            break;
        case DF_CAST_TO_UNSIGNED_SHORT:
            break;
        case DF_CAST_TO_UNSIGNED_INTEGER:
            break;
        case DF_CAST_TO_UNSIGNED_LONG:
            break;
        case DF_CAST_TO_FLOAT:
            break;
        case DF_CAST_TO_DOUBLE:
            break;
    }
    return 0;
}