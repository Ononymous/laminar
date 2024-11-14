//
// Created by Lukas Brand on 20.03.2023.
//
#include "df_logic.h"

#include "logger_system/df_operation_logger.h"

#include <stdlib.h>

int compute_not(const struct ts_value* operand, struct ts_value* result);

int compute_and(const struct ts_value* const* operands, unsigned int operand_count, struct ts_value* result);

int compute_or(const struct ts_value* const* operands, unsigned int operand_count, struct ts_value* result);

int compute_lt(const struct ts_value* const* operands, unsigned int operand_count, struct ts_value* result);

int compute_lte(const struct ts_value* const* operands, unsigned int operand_count, struct ts_value* result);

int compute_gt(const struct ts_value* const* operands, unsigned int operand_count, struct ts_value* result);

int compute_gte(const struct ts_value* const* operands, unsigned int operand_count, struct ts_value* result);

int compute_eq(const struct ts_value* const* operands, unsigned int operand_count, struct ts_value* result);

int compute_neq(const struct ts_value* const* operands, unsigned int operand_count, struct ts_value* result);


int df_logic_operation_with_type(const enum df_logic_ops logic_operation,
                                 const struct ts_value* const* operands,
                                 const unsigned int operand_count,
                                 const enum ts_types result_type,
                                 struct ts_value** const result) {
    if (result_type != TS_BOOLEAN) {
        log_result_type_ignored();
    }
    struct ts_value* result_value = malloc(sizeof(struct ts_value));
    result_value->type = TS_BOOLEAN;
    *result = result_value;

    switch (logic_operation) {
        case DF_LOGIC_NOT: {
            if (operand_count != 1) {
                log_operand_count_mismatch(1, operand_count);
                return 0;
            }
            return compute_not(operands[0], result_value);
        }
        case DF_LOGIC_AND: {
            if (operand_count < 1) {
                log_operand_count_mismatch(1, operand_count);
                return 0;
            }
            return compute_and(operands, operand_count, result_value);
        }
        case DF_LOGIC_OR: {
            if (operand_count < 1) {
                log_operand_count_mismatch(1, operand_count);
                return 0;
            }
            return compute_or(operands, operand_count, result_value);
        }
        case DF_LOGIC_LESS_THAN: {
            if (operand_count < 2) {
                log_operand_count_mismatch(2, operand_count);
                return 0;
            }
            return compute_lt(operands, operand_count, result_value);
        }
        case DF_LOGIC_LESS_THAN_EQUALS: {
            if (operand_count < 2) {
                log_operand_count_mismatch(2, operand_count);
                return 0;
            }
            return compute_lte(operands, operand_count, result_value);
        }
        case DF_LOGIC_GREATER_THAN: {
            if (operand_count < 2) {
                log_operand_count_mismatch(2, operand_count);
                return 0;
            }
            return compute_gt(operands, operand_count, result_value);
        }
        case DF_LOGIC_GREATER_THAN_EQUALS: {
            if (operand_count < 2) {
                log_operand_count_mismatch(2, operand_count);
                return 0;
            }
            return compute_gte(operands, operand_count, result_value);
        }
        case DF_LOGIC_EQUALS: {
            if (operand_count < 2) {
                log_operand_count_mismatch(2, operand_count);
                return 0;
            }
            return compute_eq(operands, operand_count, result_value);
        }
        case DF_LOGIC_NOT_EQUALS: {
            if (operand_count < 2) {
                log_operand_count_mismatch(2, operand_count);
                return 0;
            }
            return compute_neq(operands, operand_count, result_value);
        }
        default:
            log_operation_not_existing("LOGIC", logic_operation);
            return 0;
    }
}


int compute_not(const struct ts_value* const operand, struct ts_value* const result) {
    const enum ts_types operand_type = operand->type;
    switch (operand_type) {
        case TS_BOOLEAN:
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            result->value.ts_int = !operand->value.ts_int;
            return 1;
        }
        case TS_LONG: {
            result->value.ts_int = !operand->value.ts_long;
            return 1;
        }
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            result->value.ts_int = !operand->value.ts_unsigned_int;
            return 1;
        }
        case TS_UNSIGNED_LONG: {
            result->value.ts_int = !operand->value.ts_unsigned_long;
            return 1;
        }
        case TS_FLOAT: {
            result->value.ts_int = !operand->value.ts_float;
            return 1;
        }
        case TS_DOUBLE: {
            result->value.ts_int = !operand->value.ts_double;
            return 1;
        }
        default:
            log_unsupported_type_on_operation(operand_type, "NOT");
            return 0;
    }
}

int compute_and(const struct ts_value* const* operands,
                const unsigned int operand_count,
                struct ts_value* const result) {
    int value = 1;
    for (unsigned int i = 0; i < operand_count; i++) {
        const enum ts_types operand_type = operands[i]->type;
        switch (operand_type) {
            case TS_BOOLEAN:
            case TS_BYTE:
            case TS_SHORT:
            case TS_INTEGER:
                value = value && operands[i]->value.ts_int;
                break;
            case TS_LONG:
                value = value && operands[i]->value.ts_long;
                break;
            case TS_UNSIGNED_BYTE:
            case TS_UNSIGNED_SHORT:
            case TS_UNSIGNED_INTEGER:
                value = value && operands[i]->value.ts_unsigned_int;
                break;
            case TS_UNSIGNED_LONG:
                value = value && operands[i]->value.ts_unsigned_long;
                break;
            case TS_FLOAT:
                value = value && operands[i]->value.ts_float;
            case TS_DOUBLE:
                value = value && operands[i]->value.ts_double;
                break;
            default:
                log_unsupported_type_on_operation(operand_type, "AND");
                return 0;
        }
    }
    result->value.ts_int = value;
    return 1;
}

int compute_or(const struct ts_value* const* operands,
               const unsigned int operand_count,
               struct ts_value* const result) {
    int value = 0;
    for (unsigned int i = 0; i < operand_count; i++) {
        const enum ts_types operand_type = operands[i]->type;
        switch (operand_type) {
            case TS_BOOLEAN:
            case TS_BYTE:
            case TS_SHORT:
            case TS_INTEGER:
                value = value || operands[i]->value.ts_int;
                break;
            case TS_LONG:
                value = value || operands[i]->value.ts_long;
                break;
            case TS_UNSIGNED_BYTE:
            case TS_UNSIGNED_SHORT:
            case TS_UNSIGNED_INTEGER:
                value = value || operands[i]->value.ts_unsigned_int;
                break;
            case TS_UNSIGNED_LONG:
                value = value || operands[i]->value.ts_unsigned_long;
                break;
            case TS_FLOAT:
                value = value || operands[i]->value.ts_float;
                break;
            case TS_DOUBLE:
                value = value || operands[i]->value.ts_double;
                break;
            default:
                log_unsupported_type_on_operation(operand_type, "OR");
                return 0;
        }
    }
    result->value.ts_int = value;
    return 1;
}

/**
 * Currently only supports comparisons in range and precision of double
 * @param operands
 * @param operand_count
 * @param result
 * @return
 */
int compute_lt(const struct ts_value* const* operands,
               const unsigned int operand_count,
               struct ts_value* const result) {
    double value;
    int is_smaller = 1;
    enum ts_types operand_type = operands[0]->type;
    switch (operand_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            value = operands[0]->value.ts_int;
            break;
        case TS_LONG:
            value = operands[0]->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            value = operands[0]->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            value = operands[0]->value.ts_unsigned_long;
            break;
        case TS_FLOAT:
            value = operands[0]->value.ts_float;
        case TS_DOUBLE:
            value = operands[0]->value.ts_double;
            break;
        case TS_TIMESTAMP:
            value = operands[0]->value.ts_long;
            break;
        default:
            log_unsupported_type_on_operation(operand_type, "LESS THAN");
            return 0;
    }
    for (unsigned int i = 1; i < operand_count; i++) {
        operand_type = operands[i]->type;
        switch (operand_type) {
            case TS_BYTE:
            case TS_SHORT:
            case TS_INTEGER: {
                if (value < operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_LONG: {
                if (value < operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_UNSIGNED_BYTE:
            case TS_UNSIGNED_SHORT:
            case TS_UNSIGNED_INTEGER: {
                if (value < operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_UNSIGNED_LONG: {
                if (value < operands[i]->value.ts_unsigned_long) {
                    value = operands[i]->value.ts_unsigned_long;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_FLOAT: {
                if (value < operands[i]->value.ts_float) {
                    value = operands[i]->value.ts_float;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_DOUBLE: {
                if (value < operands[i]->value.ts_double) {
                    value = operands[i]->value.ts_double;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_TIMESTAMP: {
                if (value < operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            default:
                log_unsupported_type_on_operation(operand_type, "LESS THAN");
                return 0;
        }
        if (!is_smaller) {
            break;
        }
    }
    result->value.ts_int = is_smaller;
    return 1;
}

/**
 * Currently only supports comparisons in range and precision of double
 * @param operands
 * @param operand_count
 * @param result
 * @return
 */
int compute_lte(const struct ts_value* const* operands,
                const unsigned int operand_count,
                struct ts_value* const result) {
    double value;
    int is_smaller = 1;
    enum ts_types operand_type = operands[0]->type;
    switch (operand_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            value = operands[0]->value.ts_int;
            break;
        case TS_LONG:
            value = operands[0]->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            value = operands[0]->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            value = operands[0]->value.ts_unsigned_long;
            break;
        case TS_FLOAT:
            value = operands[0]->value.ts_float;
            break;
        case TS_DOUBLE:
            value = operands[0]->value.ts_double;
            break;
        case TS_TIMESTAMP:
            value = operands[0]->value.ts_long;
            break;
        default:
            log_unsupported_type_on_operation(operand_type, "LESS THAN EQUALS");
            return 0;
    }
    for (unsigned int i = 1; i < operand_count; i++) {
        operand_type = operands[i]->type;
        switch (operand_type) {
            case TS_BYTE:
            case TS_SHORT:
            case TS_INTEGER: {
                if (value <= operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_LONG: {
                if (value <= operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_UNSIGNED_BYTE:
            case TS_UNSIGNED_SHORT:
            case TS_UNSIGNED_INTEGER: {
                if (value <= operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_UNSIGNED_LONG: {
                if (value <= operands[i]->value.ts_unsigned_long) {
                    value = operands[i]->value.ts_unsigned_long;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_FLOAT: {
                if (value <= operands[i]->value.ts_float) {
                    value = operands[i]->value.ts_float;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_DOUBLE: {
                if (value <= operands[i]->value.ts_double) {
                    value = operands[i]->value.ts_double;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            case TS_TIMESTAMP: {
                if (value <= operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_smaller = 0;
                }
                break;
            }
            default:
                log_unsupported_type_on_operation(operand_type, "LESS THAN EQUALS");
                return 0;
        }
        if (!is_smaller) {
            break;
        }
    }
    result->value.ts_int = is_smaller;
    return 1;
}

/**
 * Currently only supports comparisons in range and precision of double
 * @param operands
 * @param operand_count
 * @param result
 * @return
 */
int compute_gt(const struct ts_value* const* operands,
               const unsigned int operand_count,
               struct ts_value* const result) {
    double value;
    int is_bigger = 1;
    enum ts_types operand_type = operands[0]->type;
    switch (operand_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            value = operands[0]->value.ts_int;
            break;
        case TS_LONG:
            value = operands[0]->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            value = operands[0]->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            value = operands[0]->value.ts_unsigned_long;
            break;
        case TS_FLOAT:
            value = operands[0]->value.ts_float;
            break;
        case TS_DOUBLE:
            value = operands[0]->value.ts_double;
            break;
        case TS_TIMESTAMP:
            value = operands[0]->value.ts_long;
            break;
        default:
            log_unsupported_type_on_operation(operand_type, "GREATER THAN");
            return 0;
    }
    for (unsigned int i = 1; i < operand_count; i++) {
        operand_type = operands[i]->type;
        switch (operand_type) {
            case TS_BYTE:
            case TS_SHORT:
            case TS_INTEGER: {
                if (value > operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_LONG: {
                if (value > operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_UNSIGNED_BYTE:
            case TS_UNSIGNED_SHORT:
            case TS_UNSIGNED_INTEGER: {
                if (value > operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_UNSIGNED_LONG: {
                if (value > operands[i]->value.ts_unsigned_long) {
                    value = operands[i]->value.ts_unsigned_long;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_FLOAT: {
                if (value > operands[i]->value.ts_float) {
                    value = operands[i]->value.ts_float;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_DOUBLE: {
                if (value > operands[i]->value.ts_double) {
                    value = operands[i]->value.ts_double;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_TIMESTAMP: {
                if (value > operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            default:
                log_unsupported_type_on_operation(operand_type, "GREATER THAN");
                return 0;
        }
        if (!is_bigger) {
            break;
        }
    }
    result->value.ts_int = is_bigger;
    return 1;
}

/**
 * Currently only supports comparisons in range and precision of double
 * @param operands
 * @param operand_count
 * @param result
 * @return
 */
int compute_gte(const struct ts_value* const* operands,
                const unsigned int operand_count,
                struct ts_value* const result) {
    double value;
    int is_bigger = 1;
    enum ts_types operand_type = operands[0]->type;
    switch (operand_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            value = operands[0]->value.ts_int;
            break;
        case TS_LONG:
            value = operands[0]->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            value = operands[0]->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            value = operands[0]->value.ts_unsigned_long;
            break;
        case TS_FLOAT:
            value = operands[0]->value.ts_float;
            break;
        case TS_DOUBLE:
            value = operands[0]->value.ts_double;
            break;
        case TS_TIMESTAMP:
            value = operands[0]->value.ts_long;
            break;
        default:
            log_unsupported_type_on_operation(operand_type, "GRATER THAN EQUALS");
            return 0;
    }
    for (unsigned int i = 1; i < operand_count; i++) {
        operand_type = operands[i]->type;
        switch (operand_type) {
            case TS_BYTE:
            case TS_SHORT:
            case TS_INTEGER: {
                if (value >= operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_LONG: {
                if (value >= operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_UNSIGNED_BYTE:
            case TS_UNSIGNED_SHORT:
            case TS_UNSIGNED_INTEGER: {
                if (value >= operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_UNSIGNED_LONG: {
                if (value >= operands[i]->value.ts_unsigned_long) {
                    value = operands[i]->value.ts_unsigned_long;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_FLOAT: {
                if (value >= operands[i]->value.ts_float) {
                    value = operands[i]->value.ts_float;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_DOUBLE: {
                if (value >= operands[i]->value.ts_double) {
                    value = operands[i]->value.ts_double;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            case TS_TIMESTAMP: {
                if (value >= operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_bigger = 0;
                }
                break;
            }
            default:
                log_unsupported_type_on_operation(operand_type, "GREATER THAN EQUALS");
                return 0;
        }
        if (!is_bigger) {
            break;
        }
    }
    result->value.ts_int = is_bigger;
    return 1;
}

/**
 * Currently only supports comparisons in range and precision of double
 * @param operands
 * @param operand_count
 * @param result
 * @return
 */
int compute_eq(const struct ts_value* const* operands,
               const unsigned int operand_count,
               struct ts_value* const result) {
    double value;
    int is_equal = 1;
    enum ts_types operand_type = operands[0]->type;
    switch (operand_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            value = operands[0]->value.ts_int;
            break;
        case TS_LONG:
            value = operands[0]->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            value = operands[0]->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            value = operands[0]->value.ts_unsigned_long;
            break;
        case TS_FLOAT:
            value = operands[0]->value.ts_float;
            break;
        case TS_DOUBLE:
            value = operands[0]->value.ts_double;
            break;
        case TS_TIMESTAMP:
            value = operands[0]->value.ts_long;
            break;
        default:
            log_unsupported_type_on_operation(operand_type, "EQUALS");
            return 0;
    }
    for (unsigned int i = 1; i < operand_count; i++) {
        operand_type = operands[i]->type;
        switch (operand_type) {
            case TS_BYTE:
            case TS_SHORT:
            case TS_INTEGER: {
                if (value == operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_equal = 0;
                }
                break;
            }
            case TS_LONG: {
                if (value == operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_equal = 0;
                }
                break;
            }
            case TS_UNSIGNED_BYTE:
            case TS_UNSIGNED_SHORT:
            case TS_UNSIGNED_INTEGER: {
                if (value == operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_equal = 0;
                }
                break;
            }
            case TS_UNSIGNED_LONG: {
                if (value == operands[i]->value.ts_unsigned_long) {
                    value = operands[i]->value.ts_unsigned_long;
                } else {
                    is_equal = 0;
                }
                break;
            }
            case TS_FLOAT: {
                if (value == operands[i]->value.ts_float) {
                    value = operands[i]->value.ts_float;
                } else {
                    is_equal = 0;
                }
                break;
            }
            case TS_DOUBLE: {
                if (value == operands[i]->value.ts_double) {
                    value = operands[i]->value.ts_double;
                } else {
                    is_equal = 0;
                }
                break;
            }
            case TS_TIMESTAMP: {
                if (value == operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_equal = 0;
                }
                break;
            }
            default:
                log_unsupported_type_on_operation(operand_type, "EQUALS");
                return 0;
        }
        if (!is_equal) {
            break;
        }
    }
    result->value.ts_int = is_equal;
    return 1;
}

/**
 * Currently only supports comparisons in range and precision of double
 * @param operands
 * @param operand_count
 * @param result
 * @return
 */
int compute_neq(const struct ts_value* const* operands,
                const unsigned int operand_count,
                struct ts_value* const result) {
    double value;
    int is_not_equal = 1;
    enum ts_types operand_type = operands[0]->type;
    switch (operand_type) {
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            value = operands[0]->value.ts_int;
            break;
        case TS_LONG:
            value = operands[0]->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            value = operands[0]->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            value = operands[0]->value.ts_unsigned_long;
            break;
        case TS_FLOAT:
            value = operands[0]->value.ts_float;
            break;
        case TS_DOUBLE:
            value = operands[0]->value.ts_double;
            break;
        case TS_TIMESTAMP:
            value = operands[0]->value.ts_long;
            break;
        default:
            log_unsupported_type_on_operation(operand_type, "EQUALS");
            return 0;
    }
    for (unsigned int i = 1; i < operand_count; i++) {
        operand_type = operands[i]->type;
        switch (operand_type) {
            case TS_BYTE:
            case TS_SHORT:
            case TS_INTEGER: {
                if (value != operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_not_equal = 0;
                }
                break;
            }
            case TS_LONG: {
                if (value != operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_not_equal = 0;
                }
                break;
            }
            case TS_UNSIGNED_BYTE:
            case TS_UNSIGNED_SHORT:
            case TS_UNSIGNED_INTEGER: {
                if (value != operands[i]->value.ts_int) {
                    value = operands[i]->value.ts_int;
                } else {
                    is_not_equal = 0;
                }
                break;
            }
            case TS_UNSIGNED_LONG: {
                if (value != operands[i]->value.ts_unsigned_long) {
                    value = operands[i]->value.ts_unsigned_long;
                } else {
                    is_not_equal = 0;
                }
                break;
            }
            case TS_FLOAT: {
                if (value != operands[i]->value.ts_float) {
                    value = operands[i]->value.ts_float;
                } else {
                    is_not_equal = 0;
                }
                break;
            }
            case TS_DOUBLE: {
                if (value != operands[i]->value.ts_double) {
                    value = operands[i]->value.ts_double;
                } else {
                    is_not_equal = 0;
                }
                break;
            }
            case TS_TIMESTAMP: {
                if (value != operands[i]->value.ts_long) {
                    value = operands[i]->value.ts_long;
                } else {
                    is_not_equal = 0;
                }
                break;
            }
            default:
                log_unsupported_type_on_operation(operand_type, "NOT EQUALS");
                return 0;
        }
        if (!is_not_equal) {
            break;
        }
    }
    result->value.ts_int = is_not_equal;
    return 1;
}