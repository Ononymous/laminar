//
// Created by Lukas Brand on 21.03.23.
//

#include "logger_system/df_operation_logger.h"

#include "logger_system/df_logger.h"

#include <stdlib.h>

void log_result_type_ignored() {
    log_info("Result type is ignored");
}

void log_operation_not_existing(const char* operation_category, unsigned int operation) {
    // TODO
}

void log_unsupported_type() {
    // TODO
}

void log_unsupported_type_on_operation(enum ts_types type, const char* operation_name) {
    char* type_string = value_type_as_string((type));
    log_error("Operation %s does not support type %s", operation_name, type_string);
    free(type_string);
}

void log_type_mismatch(const enum ts_types result_type, const enum ts_types current_type) {
    char* result_type_string = value_type_as_string((result_type));
    char* operand_type_string = value_type_as_string((current_type));
    log_error("Types mismatch: %s (operation_type) / %s (value type)", result_type_string, operand_type_string);
    free(result_type_string);
    free(operand_type_string);
}

void log_not_a_number(const enum ts_types current_type) {
    char* operand_type_string = value_type_as_string((current_type));
    log_error("Operand is not a number: %s", operand_type_string);
    free(operand_type_string);
}

void log_operand_count_mismatch(const unsigned int expected, const unsigned int actual) {
    log_error("Input value mismatch operation input count: expected %u actual %u", expected, actual);
}


void log_reference_must_not_be_null(const char* reference) {
    log_error("Reference must not be null: %s", reference);
}