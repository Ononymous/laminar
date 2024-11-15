//
// Created by Lukas Brand on 30.03.23.
//
#ifdef ESP8266
#include "ts_types.h"
#include <stdlib.h>
#include <string.h>
#else
#include "type_system/ts_types.h"
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#endif

char* value_type_as_string(enum ts_types type) {
    char* string;
    const char* temp_string;
    switch (type) {
        case TS_UNINITIALIZED:
            temp_string = "TS_UNINITIALIZED";
            break;
        case TS_BOOLEAN:
            temp_string = "TS_BOOLEAN";
            break;
        case TS_BYTE:
            temp_string = "TS_BYTE";
            break;
        case TS_SHORT:
            temp_string = "TS_SHORT";
            break;
        case TS_INTEGER:
            temp_string = "TS_INTEGER";
            break;
        case TS_LONG:
            temp_string = "TS_LONG";
            break;
        case TS_UNSIGNED_BYTE:
            temp_string = "TS_UNSIGNED BYTE";
            break;
        case TS_UNSIGNED_SHORT:
            temp_string = "TS_UNSIGNED SHORT";
            break;
        case TS_UNSIGNED_INTEGER:
            temp_string = "TS_UNSIGNED INTEGER";
            break;
        case TS_UNSIGNED_LONG:
            temp_string = "TS_UNSIGNED LONG";
            break;
        case TS_DOUBLE:
            temp_string = "TS_DOUBLE";
            break;
        case TS_TIMESTAMP:
            temp_string = "TS_DATETIME";
            break;
	case TS_PRIM_STRING:
	    temp_string = "TS_PRIM_STRING";
	    break;
#ifndef ESP8266
        case TS_PRIM_LARGE_STRING:
        temp_string = "TS_PRIM_LARGE_STRING";
        break;
        case TS_PRIM_2D_DOUBLE_ARRAY:
            temp_string = "TS_PRIM_2D_DOUBLE_ARRAY";
            break;
        case TS_PRIM_DOUBLE_ARRAY:
            temp_string = "TS_PRIM_DOUBLE_ARRAY";
            break;
        case TS_STRING:
            temp_string = "TS_STRING";
            break;
        case TS_ARRAY:
            temp_string = "TS_ARRAY";
            break;
        case TS_LIST:
            temp_string = "TS_LIST";
            break;
        case TS_QUEUE:
            temp_string = "TS_STREAM";
            break;
        case TS_RECORD:
            temp_string = "TS_RECORD";
            break;
#endif
        default:
            temp_string = "TS_UNRECOGNIZED TYPE";
    }
    string = malloc(strlen(temp_string) + 1);
    strcpy(string, temp_string);
    return string;
}

#ifndef ESP8266
char* prim_double_array_as_string(const double (*data)[TS_PRIM_DOUBLE_ARRAY_SIZE]) {
    char* array_value_string;
    int array_value_string_size = 0;
    char* element_strings[TS_PRIM_DOUBLE_ARRAY_SIZE];
    for (size_t i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        const double element_value = (* data)[i];
        char* const element_value_format_string = "%f";
        const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
        char* const element_value_string = malloc(element_value_string_size + 1);
        const int ret = snprintf(element_value_string,
                                    element_value_string_size + 1,
                                    element_value_format_string,
                                    element_value);
        if (ret < 0 || ret > element_value_string_size) {
            free(element_value_string);
            return NULL;
        }

        const char* element_format_string;
        if(i == TS_PRIM_DOUBLE_ARRAY_SIZE - 1) {
            element_format_string = "%s\n";
        } else {
            element_format_string = "%s, ";
        }
        const int element_string_size = snprintf(NULL, 0, element_format_string, element_value_string);
        char* const element_string = malloc(element_string_size + 1);
        const int ret2 = snprintf(
            element_string, element_string_size + 1, element_format_string, element_value_string);
        if (ret2 < 0 || ret2 > element_string_size) {
            free(element_string);
            return NULL;
        }
        free(element_value_string);

        array_value_string_size += element_string_size;
        element_strings[i] = element_string;
    }
    array_value_string = malloc(array_value_string_size + 1);
    for (size_t i = 0; i < TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
        int offset = (i == 0) ? 0 : strlen(array_value_string);
        const int ret = snprintf(array_value_string + offset, (array_value_string_size + 1) - offset, "%s", element_strings[i]);
        if (ret < 0 || ret > array_value_string_size) {
            free(array_value_string);
            return NULL;
        }
        free(element_strings[i]);
    }

    return array_value_string;
}

char* prim_2d_double_array_as_string(const double (*data)[TS_PRIM_2D_DOUBLE_ARRAY_ROWS][TS_PRIM_2D_DOUBLE_ARRAY_COLS]) {
    char* array_value_string;

    int array_value_string_size = 0;
    char* element_strings[TS_PRIM_2D_DOUBLE_ARRAY_ROWS][TS_PRIM_2D_DOUBLE_ARRAY_COLS];

    for (size_t i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
        for (size_t j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            const double element_value = (* data)[i][j];

            char* const element_value_format_string = "%f";
            const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
            char* const element_value_string = malloc(element_value_string_size + 1);
            const int ret = snprintf(element_value_string,
                                        element_value_string_size + 1,
                                        element_value_format_string,
                                        element_value);
            if (ret < 0 || ret > element_value_string_size) {
                free(element_value_string);
                return NULL;
            }

            const char* element_format_string;
            // adding the extra space for a new line between rows
            if(j == TS_PRIM_2D_DOUBLE_ARRAY_COLS - 1) {
                element_format_string = "%s\n";
            } else {
                element_format_string = "%s, ";
            }
            const int element_string_size = snprintf(NULL, 0, element_format_string, element_value_string);
            char* const element_string = malloc(element_string_size + 1);
            const int ret2 = snprintf(
                element_string, element_string_size + 1, element_format_string, element_value_string);
            if (ret2 < 0 || ret2 > element_string_size) {
                free(element_string);
                return NULL;
            }
            free(element_value_string);

            array_value_string_size += element_string_size;
            element_strings[i][j] = element_string;
        }
    }

    array_value_string = malloc(array_value_string_size + 1);
    for (size_t i = 0; i < TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
        for (size_t j = 0; j < TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
            int offset = (i == 0 && j == 0) ? 0 : strlen(array_value_string);
            const int ret = snprintf(array_value_string + offset, (array_value_string_size + 1) - offset, "%s", element_strings[i][j]);
            if (ret < 0 || ret > array_value_string_size) {
                free(array_value_string);
                return NULL;
            }
            free(element_strings[i][j]);
        }
    }

    return array_value_string;
}

char* array_as_string(const struct ts_value_array* value) { // NOLINT(misc-no-recursion)
    const struct ts_value_array array_value = *value;

    char* array_value_string;
    if (array_value.value == NULL) {
        array_value_string = "NULL (not loaded)";
    } else {
        switch (array_value.type) {
            case TS_ERROR: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const uint32_t element_value = ((uint32_t*)array_value.value)[i];

                    char* const element_value_format_string = "%u";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_EXCEPTION: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const uint32_t element_value = ((uint32_t*)array_value.value)[i];

                    char* const element_value_format_string = "%u";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_BOOLEAN: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const bool element_value = ((bool*)array_value.value)[i];

                    const char* const bool_element_value = (element_value) ? "true" : "false";

                    char* const element_value_format_string = "%s";
                    const int element_value_string_size =
                        snprintf(NULL, 0, element_value_format_string, bool_element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             bool_element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_BYTE: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const int8_t element_value = ((int8_t*)array_value.value)[i];

                    char* const element_value_format_string = "%d";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_SHORT: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const int16_t element_value = ((int16_t*)array_value.value)[i];

                    char* const element_value_format_string = "%d";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_INTEGER: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const int32_t element_value = ((int32_t*)array_value.value)[i];

                    char* const element_value_format_string = "%d";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_LONG: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const int64_t element_value = ((int64_t*)array_value.value)[i];

                    char* const element_value_format_string = "%ld";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_UNSIGNED_BYTE: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const uint8_t element_value = ((uint8_t*)array_value.value)[i];

                    char* const element_value_format_string = "%u";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_UNSIGNED_SHORT: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const uint16_t element_value = ((uint16_t*)array_value.value)[i];

                    char* const element_value_format_string = "%u";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_UNSIGNED_INTEGER: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const uint32_t element_value = ((uint32_t*)array_value.value)[i];

                    char* const element_value_format_string = "%u";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_UNSIGNED_LONG: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const uint64_t element_value = ((uint64_t*)array_value.value)[i];

                    char* const element_value_format_string = "%lu";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_FLOAT: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const float element_value = ((float*)array_value.value)[i];

                    char* const element_value_format_string = "%f";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_DOUBLE: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const double element_value = ((double*)array_value.value)[i];

                    char* const element_value_format_string = "%f";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_TIMESTAMP: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const int64_t element_value = ((int64_t*)array_value.value)[i];

                    char* const element_value_format_string = "%ld";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_STRING: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const struct ts_value_string element_value = ((const struct ts_value_string*)array_value.value)[i];

                    char* const element_value_format_string = "%s";
                    const int element_value_string_size =
                        snprintf(NULL, 0, element_value_format_string, element_value.value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value.value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]: %s, ";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            case TS_ARRAY: {
                int array_value_string_size = 0;
                char* element_strings[array_value.size];

                for (size_t i = 0; i < array_value.size; i++) {
                    const struct ts_value_array* const array_element = (struct ts_value_array*)array_value.value;
                    char* element_value = array_as_string(&array_element[i]);

                    char* const element_value_format_string = "%s";
                    const int element_value_string_size = snprintf(NULL, 0, element_value_format_string, element_value);
                    char* const element_value_string = malloc(element_value_string_size + 1);
                    const int ret = snprintf(element_value_string,
                                             element_value_string_size + 1,
                                             element_value_format_string,
                                             element_value);
                    if (ret < 0 || ret > element_value_string_size) {
                        free(element_value_string);
                        return NULL;
                    }

                    const char* const element_format_string = "[%lu]:\n%s\n";
                    const int element_string_size = snprintf(NULL, 0, element_format_string, i, element_value_string);
                    char* const element_string = malloc(element_string_size + 1);
                    const int ret2 = snprintf(
                        element_string, element_string_size + 1, element_format_string, i, element_value_string);
                    if (ret2 < 0 || ret2 > element_string_size) {
                        free(element_string);
                        return NULL;
                    }
                    free(element_value_string);

                    array_value_string_size += element_string_size;
                    element_strings[i] = element_string;
                }

                array_value_string = malloc(array_value_string_size + 1);
                for (size_t i = 0; i < array_value.size; i++) {
                    const int ret =
                        snprintf(array_value_string + ((i == 0) ? 0 : strlen(array_value_string)),
                                 (array_value_string_size + 1) - ((i == 0) ? 0 : strlen(array_value_string)),
                                 "%s",
                                 element_strings[i]);
                    if (ret < 0 || ret > array_value_string_size) {
                        free(array_value_string);
                        return NULL;
                    }
                    free(element_strings[i]);
                }
            } break;
            default:
                array_value_string = "VALUE PRINT NOT SUPPORTED";
        }
    }

    const struct ts_storage_system storage_system = array_value.storage_system;
    char uuid_string[37];
    uuid_unparse(storage_system.id, uuid_string);
    //const size_t element_size = storage_system.element_size;

    const char* const header_string = "TS_ARRAY:";
    const char* const storage_system_string = "[Storage System:[UUID:%s]]"; //[element size:%lu]
    const char* const size_string = "[size:%lu]";
    char* const type_string = value_type_as_string(array_value.type);
    const char* const type_format_string = "[type:%s]";
    const char* const array_value_format_string = "[value:[%s]]";

    char* array_string = NULL;
    int array_string_size = strlen(header_string);
    array_string_size += snprintf(NULL, 0, storage_system_string, uuid_string);
    array_string_size += snprintf(NULL, 0, size_string, array_value.size);
    array_string_size += snprintf(NULL, 0, type_format_string, type_string);
    array_string_size += snprintf(NULL, 0, array_value_format_string, array_value_string);

    array_string = malloc(array_string_size + 1);
    strncpy(array_string, header_string, array_string_size + 1);
    const int ret2 = snprintf(array_string + strlen(array_string),
                              (array_string_size + 1) - strlen(array_string),
                              storage_system_string,
                              uuid_string);
    const int ret3 = snprintf(array_string + strlen(array_string),
                              (array_string_size + 1) - strlen(array_string),
                              size_string,
                              array_value.size);
    const int ret4 = snprintf(array_string + strlen(array_string),
                              (array_string_size + 1) - strlen(array_string),
                              type_format_string,
                              type_string);

    free(type_string);
    const int ret5 = snprintf(
        array_string + strlen(array_string), array_string_size + 1, array_value_format_string, array_value_string);
    if (array_value.value != NULL) {
        free(array_value_string);
    }
    if ((ret2 < 0 || ret2 > array_string_size) && (ret3 < 0 || ret3 > array_string_size) &&
        (ret4 < 0 || ret4 > array_string_size) && (ret5 < 0 || ret5 > array_string_size)) {
        free(array_string);
        return NULL;
    }
    return array_string;
}


char* value_as_string(const struct ts_value* value) {
    if (value == NULL) {
        return "NULL";
    }

    char* value_string = NULL;
    int value_string_size;
    switch (value->type) {
        case TS_UNINITIALIZED: {
            const char* const format_string = "TS_UNKNOWN: UNKNOWN VALUE";
            value_string_size = snprintf(NULL, 0, "%s", format_string);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, "%s", format_string);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_BOOLEAN: {
            const char* const format_string = "TS_BOOLEAN: %d";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_int);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_int);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_BYTE: {
            const char* const format_string = "TS_BYTE: %d";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_int);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_int);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_SHORT: {
            const char* const format_string = "TS_SHORT: %d";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_int);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_int);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_INTEGER: {
            const char* const format_string = "TS_INTEGER: %d";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_int);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_int);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_LONG: {
            const char* const format_string = "TS_LONG: %ld";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_long);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_long);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_UNSIGNED_BYTE: {
            const char* const format_string = "TS_UNSIGNED_BYTE: %u";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_unsigned_int);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_unsigned_int);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_UNSIGNED_SHORT: {
            const char* const format_string = "TS_UNSIGNED_SHORT: %u";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_unsigned_int);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_unsigned_int);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_UNSIGNED_INTEGER: {
            const char* const format_string = "TS_UNSIGNED_INTEGER: %u";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_unsigned_int);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_unsigned_int);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_UNSIGNED_LONG: {
            const char* const format_string = "TS_UNSIGNED_LONG: %lu";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_unsigned_long);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_unsigned_long);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_FLOAT: {
            const char* const format_string = "TS_FLOAT: %f";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_float);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_float);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_DOUBLE: {
            const char* const format_string = "TS_DOUBLE: %f";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_double);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_double);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_TIMESTAMP: {
            const char* const format_string = "TS_TIMESTAMP: %ld";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_long);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_long);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_PRIM_STRING: {
            const char* const format_string = "TS_PRIM_STRING: %s";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_prim_string);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_prim_string);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_PRIM_LARGE_STRING: {
            const char* const format_string = "TS_PRIM_LARGE_STRING: %s";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_prim_large_string);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_prim_large_string);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_PRIM_2D_DOUBLE_ARRAY: {
            const char* const format_string = "TS_PRIM_2D_DOUBLE_ARRAY: \n%s";
            char* double_array_string_value = prim_2d_double_array_as_string(&value->value.ts_prim_2d_double_array);
            value_string_size = snprintf(NULL, 0, format_string, double_array_string_value);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, double_array_string_value);
            free(double_array_string_value);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_PRIM_DOUBLE_ARRAY: {
            const char* const format_string = "TS_PRIM_DOUBLE_ARRAY: \n%s";
            char* double_array_string_value = prim_double_array_as_string(&value->value.ts_prim_double_array);
            value_string_size = snprintf(NULL, 0, format_string, double_array_string_value);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, double_array_string_value);
            free(double_array_string_value);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_STRING: {
            const char* const format_string = "TS_STRING: %s";
            value_string_size = snprintf(NULL, 0, format_string, value->value.ts_string.value);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, format_string, value->value.ts_string.value);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
        } break;
        case TS_ARRAY: {
            value_string = array_as_string(&value->value.ts_array);
        } break;
        default: {
            const char* const format_string = "VALUE PRINT NOT SUPPORTED";
            value_string_size = snprintf(NULL, 0, "%s", format_string);
            value_string = malloc(value_string_size + 1);
            const int ret = snprintf(value_string, value_string_size + 1, "%s", format_string);
            if (ret < 0 || ret > value_string_size) {
                free(value_string);
                return NULL;
            }
            break;
        }
    }
    return value_string;
}

#endif

bool value_deep_set(const struct ts_value* source, struct ts_value* destination) {
    if (destination == NULL) {
        return false;
    }

    destination->type = source->type;
    switch (source->type) {
        case TS_BOOLEAN:
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            destination->value.ts_int = source->value.ts_int;
            break;
        case TS_LONG:
            destination->value.ts_long = source->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            destination->value.ts_unsigned_int = source->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            destination->value.ts_unsigned_long = source->value.ts_unsigned_long;
            break;
        case TS_DOUBLE:
            destination->value.ts_double = source->value.ts_double;
            break;
        case TS_TIMESTAMP:
            destination->value.ts_unsigned_long = source->value.ts_unsigned_long;
            break;
        case TS_PRIM_STRING:
            strncpy(destination->value.ts_prim_string,source->value.ts_prim_string,sizeof(destination->value.ts_prim_string));
	    break;
        case TS_PRIM_LARGE_STRING:
            strncpy(destination->value.ts_prim_large_string,source->value.ts_prim_large_string,sizeof(destination->value.ts_prim_large_string));
	    break;
        case TS_PRIM_2D_DOUBLE_ARRAY:
            for (size_t i=0; i<TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
                for (size_t j=0; j<TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
                    destination->value.ts_prim_2d_double_array[i][j] = source->value.ts_prim_2d_double_array[i][j];
                }
            }
            break;
        case TS_PRIM_DOUBLE_ARRAY:
            for (size_t i=0; i<TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
                destination->value.ts_prim_double_array[i] = source->value.ts_prim_double_array[i];
            }
            break;
#ifndef ESP8266
        case TS_STRING:
            destination->value.ts_string = source->value.ts_string;
            // set_string(destination, value_to_string(source));
            break;
        case TS_ARRAY:
            destination->value.ts_array = source->value.ts_array;
            break;
        case TS_LIST:
            destination->value.ts_list = source->value.ts_list;
            break;
        case TS_QUEUE:
            destination->value.ts_queue = source->value.ts_queue;
            break;
        case TS_RECORD:
            destination->value.ts_record = source->value.ts_record;
            break;
#endif
        default:
            return false;
    }
    return true;
}


void deep_copy_array(const struct ts_value_array* source_array_value, struct ts_value_array* destination_array_value);

struct ts_value* value_deep_copy(const struct ts_value* const source) {
    if (source == NULL) {
        return NULL;
    }

    struct ts_value* destination = malloc(sizeof(struct ts_value));
    destination->type = source->type;
    switch (source->type) {
        case TS_BOOLEAN:
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER:
            destination->value.ts_int = source->value.ts_int;
            break;
        case TS_LONG:
            destination->value.ts_long = source->value.ts_long;
            break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER:
            destination->value.ts_unsigned_int = source->value.ts_unsigned_int;
            break;
        case TS_UNSIGNED_LONG:
            destination->value.ts_unsigned_long = source->value.ts_unsigned_long;
            break;
        case TS_DOUBLE:
            destination->value.ts_double = source->value.ts_double;
            break;
        case TS_TIMESTAMP:
            destination->value.ts_unsigned_long = source->value.ts_unsigned_long;
            break;
        case TS_PRIM_STRING:
	    strncpy(destination->value.ts_prim_string,source->value.ts_prim_string,sizeof(destination->value.ts_prim_string));
            break;
#ifndef ESP8266
        case TS_PRIM_LARGE_STRING:
            strncpy(destination->value.ts_prim_large_string,source->value.ts_prim_large_string,sizeof(destination->value.ts_prim_large_string));
            break;
        case TS_PRIM_2D_DOUBLE_ARRAY:
            for (size_t i=0; i<TS_PRIM_2D_DOUBLE_ARRAY_ROWS; i++) {
                for (size_t j=0; j<TS_PRIM_2D_DOUBLE_ARRAY_COLS; j++) {
                    destination->value.ts_prim_2d_double_array[i][j] = source->value.ts_prim_2d_double_array[i][j];
                }
            }
            break;
        case TS_PRIM_DOUBLE_ARRAY:
            for (size_t i=0; i<TS_PRIM_DOUBLE_ARRAY_SIZE; i++) {
                destination->value.ts_prim_double_array[i] = source->value.ts_prim_double_array[i];
            }
            break;
        case TS_STRING:
            // TODO
            return NULL;
            // destination->value.df_string = source->value.df_string;
        case TS_ARRAY: {
            const struct ts_value_array* const source_array_value = &source->value.ts_array;
            struct ts_value_array* const destination_array_value = &destination->value.ts_array;

            deep_copy_array(source_array_value, destination_array_value);
        } break;
#endif
        default:
            return NULL;
    }
    return destination;
}


#ifndef ESP8266
void deep_copy_array(const struct ts_value_array* const source_array_value, // NOLINT(misc-no-recursion)
                     struct ts_value_array* const destination_array_value) {
    struct ts_storage_system destination_storage_system;
    uuid_generate(destination_storage_system.id);
    destination_storage_system.element_size = source_array_value->storage_system.element_size;

    destination_array_value->storage_system = destination_storage_system;
    destination_array_value->size = source_array_value->size;
    destination_array_value->type = source_array_value->type;

    switch (source_array_value->type) {
        case TS_UNINITIALIZED:
            destination_array_value->value = NULL;
            break;
        case TS_BOOLEAN:
        case TS_BYTE:
        case TS_SHORT:
        case TS_INTEGER: {
            const size_t array_size = source_array_value->size;
            int32_t* array = malloc(array_size * sizeof(int32_t));

            for (size_t i = 0; i < array_size; i++) {
                array[i] = ((int32_t*)source_array_value->value)[i];
            }
            destination_array_value->value = array;
        } break;
        case TS_LONG:
        case TS_TIMESTAMP: {
            const size_t array_size = source_array_value->size;
            int64_t* array = malloc(array_size * sizeof(int64_t));

            for (size_t i = 0; i < array_size; i++) {
                array[i] = ((int64_t*)source_array_value->value)[i];
            }
            destination_array_value->value = array;
        } break;
        case TS_UNSIGNED_BYTE:
        case TS_UNSIGNED_SHORT:
        case TS_UNSIGNED_INTEGER: {
            const size_t array_size = source_array_value->size;
            uint32_t* array = malloc(array_size * sizeof(uint32_t));

            for (size_t i = 0; i < array_size; i++) {
                array[i] = ((uint32_t*)source_array_value->value)[i];
            }
            destination_array_value->value = array;
        } break;
        case TS_UNSIGNED_LONG: {
            const size_t array_size = source_array_value->size;
            uint64_t* array = malloc(array_size * sizeof(uint64_t));

            for (size_t i = 0; i < array_size; i++) {
                array[i] = ((uint64_t*)source_array_value->value)[i];
            }
            destination_array_value->value = array;
        } break;
        case TS_DOUBLE: {
            const size_t array_size = source_array_value->size;
            double* array = malloc(array_size * sizeof(double));

            for (size_t i = 0; i < array_size; i++) {
                array[i] = ((double*)source_array_value->value)[i];
            }
            destination_array_value->value = array;
        } break;
        case TS_STRING:
            destination_array_value->value = NULL;
            break;
        case TS_ARRAY: {
            const size_t source_inner_array_size = source_array_value->size;
            const struct ts_value_array* const source_inner_array_value =
                ((struct ts_value_array*)source_array_value->value);

            struct ts_value_array* destination_inner_array =
                malloc(source_inner_array_size * sizeof(struct ts_value_array));

            for (size_t i = 0; i < source_inner_array_size; i++) {
                deep_copy_array(&source_inner_array_value[i], &destination_inner_array[i]);
            }
            destination_array_value->value = destination_inner_array;
        } break;
        default:
            destination_array_value->value = NULL;
            break;
    }
}
#endif

bool value_deep_delete(struct ts_value* value) {
#ifndef ESP8266
    switch (value->type) {
        case TS_STRING:
            // free(value->value.ts_string.value);
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
        default:
            break;
    }
#endif
    free(value);
    return 1;
}
