//
// Created by Lukas Brand on 20.03.2023.
//

#include "df_internal.h"

#include "logger_system/df_logger.h"
#include "logger_system/df_operation_logger.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_error.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"

#include <stdlib.h>
#include <woofc.h>

int compute_noop(const struct ts_value* operand, struct ts_value* result);

int compute_operand(const struct ts_value* operand, struct ts_value* result);

int compute_select(const struct ts_value* const* operands,
                   unsigned int operand_count,
                   enum ts_types result_type,
                   struct ts_value* result);

/**
 * Filters values if condition is true (Value is not written to output WooF).
 * @param operands [0](TS_BOOLEAN): Filter condition, [1](T : Any): Passed value
 * @param operand_count must be 2
 * @param operation_metadata is used to modify dataflow i.e. to omit result
 * @param result_type is of type T
 * @param result is operands[1]
 * @return true on success, false otherwise
 */
int compute_filter(const struct ts_value* const* operands,
                   unsigned int operand_count,
                   struct df_operation_metadata* operation_metadata,
                   enum ts_types result_type,
                   struct ts_value* result);

int compute_offset(const struct ts_value* const* operands,
                   unsigned int operand_count,
                   struct df_operation_metadata* operation_metadata,
                   enum ts_types result_type,
                   struct ts_value* result);

int compute_flatten(const struct ts_value* operand,
                    struct df_operation_metadata* operation_metadata,
                    struct ts_value* result);


int df_internal_operation_with_type(const enum df_internal_ops internal_operation,
                                    const struct ts_value* const* operands,
                                    const unsigned int operand_count,
                                    struct df_operation_metadata* const operation_metadata,
                                    const enum ts_types result_type,
                                    struct ts_value** const result) {
    struct ts_value* result_value = malloc(sizeof(struct ts_value));
    *result = result_value;

    switch (internal_operation) {
        case DF_INTERNAL_NOOP: {
            if (operand_count != 1) {
                log_operand_count_mismatch(1, operand_count);
                return false;
            }
            return compute_noop(operands[0], result_value);
        }
        case DF_INTERNAL_OPERAND: {
            if (operand_count != 1) {
                log_operand_count_mismatch(1, operand_count);
                return false;
            }
            return compute_operand(operands[0], result_value);
        }
        case DF_INTERNAL_SELECT:
            return compute_select(operands, operand_count, result_type, result_value);
        case DF_INTERNAL_FILTER: {
            if (operand_count != 2) {
                log_operand_count_mismatch(2, operand_count);
                return false;
            }
            return compute_filter(operands, operand_count, operation_metadata, result_type, result_value);
        }
        case DF_INTERNAL_OFFSET:
            return compute_offset(operands, operand_count, operation_metadata, result_type, result_value);
        case DF_INTERNAL_FLATTEN: {
            if (operand_count != 1) {
                log_operand_count_mismatch(1, operand_count);
                return false;
            }
            return compute_flatten(operands[0], operation_metadata, result_value);
        }
        default:
            log_operation_not_existing("INTERNAL", internal_operation);
            return false;
    }
}


int compute_noop(const struct ts_value* const operand, struct ts_value* result) {
    char* operand_string = value_as_string(operand);
    log_info("NOOP: Read array (%s)", operand_string);
    free(operand_string);
    switch (operand->type) {
        case TS_STRING: {
            log_info("NOOP: String");
            char* string = value_to_string(operand);
            // USE STRING
            log_info("NOOP: %s", string);
            fflush(stdout);
            set_string(result, string);
            // free(string);
        } break;
        case TS_ARRAY: {
            switch (get_array_type(operand)) {
                case TS_BOOLEAN:
                    break;
                case TS_BYTE:
                    break;
                case TS_SHORT:
                    break;
                case TS_INTEGER:
                    break;
                case TS_LONG:
                    break;
                case TS_UNSIGNED_BYTE: {
                    uint8_t* unsigned_byte_array = value_to_unsigned_byte_array(operand);
                    const size_t unsigned_byte_array_size = get_array_size(operand);
                    // USE ARRAY
                    for (size_t i = 0; i < unsigned_byte_array_size; i++) {
                        log_info("NOOP: [%lu]: %d", i, unsigned_byte_array[i]);
                    }
                    set_unsigned_byte_array(result, unsigned_byte_array, unsigned_byte_array_size);
                    free(unsigned_byte_array);
                } break;
                case TS_UNSIGNED_SHORT:
                    break;
                case TS_UNSIGNED_INTEGER:
                    break;
                case TS_UNSIGNED_LONG:
                    break;
                case TS_FLOAT:
                    break;
                case TS_DOUBLE:
                    break;
                case TS_TIMESTAMP:
                    break;
                case TS_STRING:
                    break;
                case TS_ARRAY:
                    break;
                case TS_LIST:
                    break;
                case TS_MAP:
                    break;
                case TS_RECORD:
                    break;
                case TS_QUEUE:
                    break;
                case TS_BINARY:
                    break;
                case TS_STREAM_ARRAY:
                    break;
            }
        } break;
        default:
            break;
    }
    return true;
}

int compute_operand(const struct ts_value* const operand, struct ts_value* const result) {
    return value_deep_set(operand, result);
}

int compute_select(const struct ts_value* const* operands,
                   const unsigned int operand_count,
                   const enum ts_types result_type,
                   struct ts_value* const result) {
    const enum ts_types select_type = operands[0]->type;
    unsigned long selector;
    switch (select_type) {
        case TS_BOOLEAN:
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            if (operands[0]->value.ts_int < 0) {
                return false;
            }
            selector = operands[0]->value.ts_int;
            break;
        case TS_LONG:
            if (operands[0]->value.ts_long < 0) {
                return false;
            }
            selector = operands[0]->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            selector = operands[0]->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            selector = operands[0]->value.ts_unsigned_long;
            break;
        case TS_FLOAT: {
            int temp_selector = (int)operands[0]->value.ts_float;
            if (temp_selector < 0) {
                return false;
            }
            selector = temp_selector;
            break;
        }
        case TS_DOUBLE: {
            int temp_selector = (int)operands[0]->value.ts_double;
            if (temp_selector < 0) {
                return false;
            }
            selector = temp_selector;
            break;
        }
        default:
            log_unsupported_type_on_operation(select_type, "SELECT");
            return false;
    }
    if (selector + 1 + 1 > operand_count) {
        log_operand_count_mismatch(selector + 1, operand_count);
        return false;
    }
    const struct ts_value* selected_value = operands[selector + 1];
    const enum ts_types selected_type = selected_value->type;
    if (result_type != select_type) {
        log_type_mismatch(result_type, selected_type);
        return false;
    }
    return value_deep_set(selected_value, result);
}

int compute_filter(const struct ts_value* const* const operands,
                   const unsigned int operand_count,
                   struct df_operation_metadata* const operation_metadata,
                   const enum ts_types result_type,
                   struct ts_value* const result) {
    const struct ts_value* condition = operands[0];
    if (condition->type != TS_BOOLEAN) {
        return false;
    }
    bool filter_value = false;
    get_boolean(&filter_value, condition);

    if (filter_value) {
        operation_metadata->flow_control.write_value = false;
        set_unknown(result);
        return true;
    } else {
        operation_metadata->flow_control.write_value = true;
        return value_deep_set(operands[1], result);
    }
}

int compute_offset(const struct ts_value* const* operands,
                   const unsigned int operand_count,
                   struct df_operation_metadata* const operation_metadata,
                   const enum ts_types result_type,
                   struct ts_value* const result) {
    value_deep_set(operands[1], result);
    // TODO
    operation_metadata->flow_control.execution_iteration += (unsigned long)operands[0]->value.ts_double;
    return true;
}

struct operand {
    struct ts_value operand_value;
    unsigned long itr;
};

const char* OUTPUT_HANDLER = "output_handler";

int compute_flatten(const struct ts_value* const operand,
                    struct df_operation_metadata* const operation_metadata,
                    struct ts_value* const result) {
    operation_metadata->flow_control.write_value = false;
    const char* const output_woof = operation_metadata->flow_control.output_woof_path;

    switch (operand->type) {
        case TS_ARRAY: {
            const struct ts_value_array array_value = operand->value.ts_array;
            switch (array_value.type) {
                case TS_BOOLEAN: {
                    bool* const boolean_array = value_to_boolean_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_boolean(boolean_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(boolean_array);
                } break;
                case TS_BYTE: {
                    int8_t* const byte_array = value_to_byte_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_byte(byte_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(byte_array);
                } break;
                case TS_SHORT: {
                    int16_t* const short_array = value_to_short_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_short(short_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(short_array);
                } break;
                case TS_INTEGER: {
                    int32_t* const integer_array = value_to_integer_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_integer(integer_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(integer_array);
                } break;
                case TS_LONG: {
                    int64_t* const long_array = value_to_long_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_long(long_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(long_array);
                } break;
                case TS_UNSIGNED_BYTE: {
                    uint8_t* const unsigned_byte_array = value_to_unsigned_byte_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_unsigned_byte(unsigned_byte_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(unsigned_byte_array);
                } break;
                case TS_UNSIGNED_SHORT: {
                    uint16_t* const unsigned_short_array = value_to_unsigned_short_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_unsigned_short(unsigned_short_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(unsigned_short_array);
                } break;
                case TS_UNSIGNED_INTEGER: {
                    uint32_t* const unsigned_integer_array = value_to_unsigned_integer_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand =
                            value_from_unsigned_integer(unsigned_integer_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(unsigned_integer_array);
                } break;
                case TS_UNSIGNED_LONG: {
                    uint64_t* const unsigned_long_array = value_to_unsigned_long_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_unsigned_long(unsigned_long_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(unsigned_long_array);
                } break;
                case TS_FLOAT: {
                    float* const float_array = value_to_float_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_float(float_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(float_array);
                } break;
                case TS_DOUBLE: {
                    double* const double_array = value_to_double_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_double(double_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(double_array);
                } break;
                case TS_TIMESTAMP: {
                    int64_t* const timestamp_array = value_to_timestamp_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_timestamp(timestamp_array[i]);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        write_value(flattened_operand);
                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    free(timestamp_array);
                } break;
                case TS_STRING: {
                    char** const string_array = value_to_string_array(operand);
                    const size_t array_size = get_array_size(operand);

                    for (size_t i = 0; i < array_size; i++) {
                        const size_t latest_output_sequence = WooFGetLatestSeqno(output_woof);
                        struct ts_value* const flattened_operand = value_from_string(string_array[i]);
                        write_value(flattened_operand);
                        const struct operand output_operand = {
                            .operand_value = *flattened_operand,
                            .itr = latest_output_sequence + 1,
                        };

                        const size_t put_result = WooFPut(output_woof, OUTPUT_HANDLER, &output_operand);
                        if (WooFInvalid(put_result)) {
                            return false;
                        }
                        free(flattened_operand);
                    }
                    operation_metadata->flow_control.execution_iteration = WooFGetLatestSeqno(output_woof);
                    //free(string_array);
                } break;
                case TS_ARRAY: {
                    log_error("FLATTEN: Type not implemented yet");
                } break;
                case TS_LIST: {
                    log_error("FLATTEN: Type not implemented yet");
                } break;
                case TS_MAP: {
                    log_error("FLATTEN: Type not implemented yet");
                } break;
                case TS_RECORD: {
                    log_error("FLATTEN: Type not implemented yet");
                } break;
                case TS_QUEUE: {
                    log_error("FLATTEN: Type not implemented yet");
                } break;
                case TS_BINARY: {
                    log_error("FLATTEN: Type not implemented yet");
                } break;
                case TS_STREAM_ARRAY: {
                    log_error("FLATTEN: Type not implemented yet");
                } break;
                default:
                    break;
            }
        } break;
        case TS_LIST:
        case TS_MAP:
        case TS_QUEUE:
        case TS_STREAM_ARRAY:
            log_error("FLATTEN: Type not implemented yet");
            break;
        default:
            log_unsupported_type_on_operation(operand->type, "FLATTEN");
    }


    set_uninitialized(result);
    return true;
}