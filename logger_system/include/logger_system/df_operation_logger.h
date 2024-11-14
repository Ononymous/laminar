//
// Created by Lukas Brand on 21.03.23.
//

#ifndef CSPOT_DF_OPERATION_LOGGER_H
#define CSPOT_DF_OPERATION_LOGGER_H

#include "type_system/ts_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void log_result_type_ignored();

void log_operation_not_existing(const char* operation_category, unsigned int operation);

void log_unsupported_type();

void log_unsupported_type_on_operation(enum ts_types type, const char* operation_name);

void log_type_mismatch(enum ts_types result_type, enum ts_types current_type);

void log_not_a_number(enum ts_types current_type);

void log_operand_count_mismatch(unsigned int expected, unsigned int actual);

void log_reference_must_not_be_null(const char* reference);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_DF_OPERATION_LOGGER_H
