//
// Created by Lukas Brand on 03.01.2023.
//
#include "logger_system/df_logger.h"

#include "logger_system/df_logger_settings.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uuid/uuid.h>

const char* PREFIX = "Logger";
int random_number;

void log_format(const char* tag, const char* message, va_list args) {
    time_t t = time(NULL);
    struct tm time = *gmtime(&t);
    if (random_number != -1) {
        printf("%d-%02d-%02d %02d:%02d:%02d %s [%s-%05d] ",
               time.tm_year + 1900,
               time.tm_mon + 1,
               time.tm_mday,
               time.tm_hour,
               time.tm_min,
               time.tm_sec,
               tag,
               PREFIX,
               random_number % 100000);
    } else {
        printf("%d-%02d-%02d %02d:%02d:%02d %s [%s] ",
               time.tm_year + 1900,
               time.tm_mon + 1,
               time.tm_mday,
               time.tm_hour,
               time.tm_min,
               time.tm_sec,
               tag,
               PREFIX);
    }
    vprintf(message, args);
    printf("\n");
}

void log_format_value(const char* tag, const char* value) {
    time_t t = time(NULL);
    struct tm time = *gmtime(&t);
    if (random_number != -1) {
        printf("%d-%02d-%02d %02d:%02d:%02d %s [%s-%05d] -> VALUE: ",
               time.tm_year + 1900,
               time.tm_mon + 1,
               time.tm_mday,
               time.tm_hour,
               time.tm_min,
               time.tm_sec,
               tag,
               PREFIX,
               random_number % 100000);
    } else {
        printf("%d-%02d-%02d %02d:%02d:%02d %s [%s] -> VALUE: ",
               time.tm_year + 1900,
               time.tm_mon + 1,
               time.tm_mday,
               time.tm_hour,
               time.tm_min,
               time.tm_sec,
               tag,
               PREFIX);
    }
    printf("%s", value);
    printf("\n");
}


void log_init(const char* prefix, bool with_random_number) {
#ifdef LOGGER_ENABLED
    PREFIX = prefix;
    if (with_random_number) {
        uuid_t uuid_seed;
        uuid_generate(uuid_seed);
        unsigned long seed = 0;
        int i;
        for (i = 0; i < 8; i++) {
            seed |= (unsigned long)uuid_seed[i] << (i * 8);
        }
        srand(seed);            // NOLINT(cert-msc51-cpp)
        random_number = rand(); // NOLINT(cert-msc50-cpp)
    } else {
        random_number = -1;
    }
#endif
}


void log_trace(const char* message, ...) {
#ifdef LOGGER_ENABLED
    if (CURRENT_LOG_LEVEL > DF_TRACE) {
        return;
    }
    va_list args;
    va_start(args, message);
    log_format("[trace]", message, args);
    va_end(args);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}

void log_trace_with_value(const char* message, const struct ts_value* value, ...) {
#ifdef LOGGER_ENABLED
    if (CURRENT_LOG_LEVEL > DF_TRACE) {
        return;
    }
    va_list args;
    va_start(args, value);
    log_format("[trace]", message, args);
    va_end(args);
    char* const value_string = value_as_string(value);
    log_format_value("[trace]", value_string);
    free(value_string);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}


void log_debug(const char* message, ...) {
#ifdef LOGGER_ENABLED
    if (CURRENT_LOG_LEVEL > DF_DEBUG) {
        return;
    }
    va_list args;
    va_start(args, message);
    log_format("[debug]", message, args);
    va_end(args);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}

void log_debug_with_value(const char* message, const struct ts_value* value, ...) {
#ifdef LOGGER_ENABLED
    if (CURRENT_LOG_LEVEL > DF_DEBUG) {
        return;
    }
    va_list args;
    va_start(args, value);
    log_format("[debug]", message, args);
    va_end(args);
    char* const value_string = value_as_string(value);
    log_format_value("[debug]", value_string);
    free(value_string);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}


void log_info(const char* message, ...) {
#ifdef LOGGER_ENABLED
    if (CURRENT_LOG_LEVEL > DF_INFO) {
        return;
    }
    va_list args;
    va_start(args, message);
    log_format("[info] ", message, args);
    va_end(args);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}

void log_info_with_value(const char* message, const struct ts_value* value, ...) {
#ifdef LOGGER_ENABLED
    if (CURRENT_LOG_LEVEL > DF_INFO) {
        return;
    }
    va_list args;
    va_start(args, value);
    log_format("[info] ", message, args);
    va_end(args);
    char* const value_string = value_as_string(value);
    log_format_value("[info] ", value_string);
    free(value_string);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}


void log_warn(const char* message, ...) {
#ifdef LOGGER_ENABLED
    if (CURRENT_LOG_LEVEL > DF_WARN) {
        return;
    }
    va_list args;
    va_start(args, message);
    log_format("[warn] ", message, args);
    va_end(args);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}

void log_warn_with_value(const char* message, const struct ts_value* value, ...) {
#ifdef LOGGER_ENABLED
    if (CURRENT_LOG_LEVEL > DF_WARN) {
        return;
    }
    va_list args;
    va_start(args, value);
    log_format("[warn] ", message, args);
    va_end(args);
    char* const value_string = value_as_string(value);
    log_format_value("[warn] ", value_string);
    free(value_string);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}


void log_error(const char* message, ...) {
#ifdef LOGGER_ENABLED
    va_list args;
    va_start(args, message);
    log_format("[error]", message, args);
    va_end(args);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}

void log_error_with_value(const char* message, const struct ts_value* value, ...) {
#ifdef LOGGER_ENABLED
    va_list args;
    va_start(args, value);
    log_format("[error]", message, args);
    va_end(args);
    char* const value_string = value_as_string(value);
    log_format_value("[error]", value_string);
    free(value_string);
#ifdef DIRECT_FLUSH
    fflush(stdout);
#endif
#endif
}

/*
size_t get_print_string(const struct ts_value* value, char value_string[], size_t value_length);

char* values_as_string(const struct ts_value* values, const unsigned int value_count) {
    size_t required_value_size = 0;
    for (int i = 0; i < value_count; ++i) {
        required_value_size += get_print_string(&values[i], NULL, 0);
    }
    char* value_separator = ", ";
    size_t value_separator_size = strlen(value_separator) * (value_count - 1);

    size_t total_required_size = required_value_size + value_separator_size + 1;
    char* value_string = malloc(total_required_size);

    get_print_string(&values[0], value_string, total_required_size);
    for (int i = 1; i < value_count; ++i) {
        strcat(value_string, value_separator);
        size_t single_value_size = get_print_string(&values[i], NULL, 0);
        char single_value_string[single_value_size + 1];
        get_print_string(&values[i], single_value_string, single_value_size + 1);
        strcat(value_string, single_value_string);
    }
    return value_string;
}

size_t get_print_string(const struct ts_value* value, char value_string[], size_t value_length) {
    enum ts_types value_type = value->type;
    if (value_type == TS_BOOLEAN) {
        if (value->value.ts_int == 0) {
            return snprintf(value_string, value_length, "\"false\"");
        } else {
            return snprintf(value_string, value_length, "\"true\"");
        }
    } else if (value_type == TS_BYTE || value_type == TS_SHORT || value_type == TS_INTEGER) {
        return snprintf(value_string, value_length, "\"%d\"", value->value.ts_int);
    } else if (value_type == TS_LONG) {
        return snprintf(value_string, value_length, "\"%ld\"", value->value.ts_long);
    } else if (value_type == TS_UNSIGNED_BYTE || value_type == TS_UNSIGNED_SHORT || value_type == TS_UNSIGNED_INTEGER) {
        return snprintf(value_string, value_length, "\"%u\"", value->value.ts_unsigned_int);
    } else if (value_type == TS_UNSIGNED_LONG) {
        return snprintf(value_string, value_length, "\"%lu\"", value->value.ts_unsigned_long);
    } else if (value_type == TS_DOUBLE) {
        return snprintf(value_string, value_length, "\"%f\"", value->value.ts_double);
    } else if (value_type == TS_STRING) {
        return snprintf(value_string, value_length, "\"%s\"", value->value.ts_array);
    } else if (value_type == TS_TIMESTAMP) {
        return snprintf(value_string, value_length, "\"TIMESTAMP - %lu\"", value->value.ts_unsigned_long);
    } else if (value_type == TS_UNINITIALIZED) {
        return snprintf(value_string, value_length, "\"UNKNOWN VALUE - %p\"", value->value.ts_record);
    } else {
        return 0;
    }
}
*/