#include "operation_system/df_operation.h"

#include "logger_system/df_operation_logger.h"
#include "operations/df_arithmetic.h"
#include "operations/df_cast.h"
#include "operations/df_custom_operations.h"
#include "operations/df_internal.h"
#include "operations/df_linear_algebra.h"
#include "operations/df_logic.h"
#include "operations/df_machine_learning.h"
#include "operations/df_string.h"

/**
 * Operation which
 * @param operation_category
 * @param operation
 * @param operands
 * @param operand_count
 * @param result_type
 * @param result
 * @return
 */
bool df_operation_with_type(struct df_operation operation,
                           const struct ts_value* const* operands,
                           size_t operand_count,
                           struct df_operation_metadata* operation_metadata,
                            enum ts_types result_type,
                           struct ts_value** result) {
    if (result == NULL) {
        log_reference_must_not_be_null("result reference");
        return 0;
    }

    switch (operation.category) {
        case DF_CAST:
            return df_cast_operation(operation.operation, operands, operand_count, result);
        case DF_ARITHMETIC:
            return df_arithmetic_operation_with_type(operation.operation, operands, operand_count, result_type, result);
        case DF_LOGIC:
            return df_logic_operation_with_type(operation.operation, operands, operand_count, result_type, result);
        case DF_INTERNAL:
            return df_internal_operation_with_type(
                operation.operation, operands, operand_count, operation_metadata, result_type, result);
        case DF_MACHINE_LEARNING:
            return df_machine_learning_operation_with_type(
                operation.operation, operands, operand_count, operation_metadata, result_type, result);
        case DF_LINEAR_ALGEBRA:
            return df_linear_algebra_operation(operation.operation, operands, operand_count, result_type, result);
        case DF_STRING:
            return df_string_operation_with_type(operation.operation, operands, operand_count, result_type, result);
        case DF_CUSTOM:
            return df_custom_operation(
                operation.operation, operands, operand_count, operation_metadata, result_type, result);
        default:
            return 0;
    }
}
