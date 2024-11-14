//
// Created by Lukas Brand on 03.01.2023.
//

#ifndef CSPOT_DFDEBUG_H
#define CSPOT_DFDEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type_system/ts_type.h"

enum LOG_LEVELS {
    DF_TRACE,
    DF_DEBUG,
    DF_INFO,
    DF_WARN,
    DF_ERROR
};

void log_init(const char* prefix, bool random_number);

void log_trace(const char* message, ...);
void log_trace_with_value(const char* message, const struct ts_value* value, ...);
void log_debug(const char* message, ...);
void log_debug_with_value(const char* message, const struct ts_value* value, ...);
void log_info(const char* message, ...);
void log_info_with_value(const char* message, const struct ts_value* value, ...);
void log_warn(const char* message, ...);
void log_warn_with_value(const char* message, const struct ts_value* value, ...);
void log_error(const char* message, ...);
void log_error_with_value(const char* message, const struct ts_value* value, ...);

#ifdef __cplusplus
}
#endif

#endif // CSPOT_DFDEBUG_H
