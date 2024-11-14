//
// Created by Lukas Brand on 01.04.23.
//
#include "catch2/catch_test_macros.hpp"
#include "type_system/ts_types.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_matrix.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"

#include <cstdlib>


TEST_CASE("TS_BYTE") {
    struct ts_value* const op = value_from_byte(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_BYTE: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_SHORT") {
    struct ts_value* const op = value_from_short(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_SHORT: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_INTEGER") {
    struct ts_value* const op = value_from_integer(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_INTEGER: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_LONG") {
    struct ts_value* const op = value_from_long(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_LONG: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_UNSIGNED_BYTE") {
    struct ts_value* const op = value_from_byte(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_UNSIGNED_BYTE: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_UNSIGNED_SHORT") {
    struct ts_value* const op = value_from_unsigned_short(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_UNSIGNED_SHORT: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_UNSIGNED_INTEGER") {
    struct ts_value* const op = value_from_unsigned_integer(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_UNSIGNED_INTEGER: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_UNSIGNED_LONG") {
    struct ts_value* const op = value_from_unsigned_long(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_UNSIGNED_LONG: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_FLOAT") {
    struct ts_value* const op = value_from_float(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_UNSIGNED_FLOAT: 5.0") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_DOUBLE") {
    struct ts_value* const op = value_from_double(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_UNSIGNED_DOUBLE: 5.0") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_TIMESTAMP") {
    struct ts_value* const op = value_from_timestamp(5);
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_TIMESTAMP: 5") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_STRING") {
    struct ts_value* const op = value_from_string("test");
    char* const string = value_as_string(op);
    printf("%s", string);

    REQUIRE(strcmp(string, "TS_STRING: test") == 0);
    free(op);
    free(string);
}

TEST_CASE("TS_ARRAY") {

    SECTION("TS_BOOLEAN") {
        const bool boolean_array[5] = {true, true, false, false, true};
        struct ts_value* const op = value_from_boolean_array(boolean_array, 5);
        char* const string = value_as_string(op);
        printf("%s", string);

        // REQUIRE(strcmp(string, "TS_STRING: test") == 0);
        free(op);
        free(string);
    } //
    SECTION("TS_DOUBLE") {
        const double double_array[5] = {1, 2, 3, 4, 5};
        struct ts_value* const op = value_from_double_array(double_array, 5);
        char* const string = value_as_string(op);
        printf("%s", string);

        // REQUIRE(strcmp(string, "TS_STRING: test") == 0);
        free(op);
        free(string);
    } //
    SECTION("TS_STRING") {
        const char* string_array[5] = {"test1", "test2", "test3", "test4", "test5"};
        struct ts_value* const op = value_from_string_array(string_array, 5);
        char* const string = value_as_string(op);
        printf("%s", string);

        // REQUIRE(strcmp(string, "TS_STRING: test") == 0);
        free(op);
        free(string);
    } //
    SECTION("TS_ARRAY") {
        double matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        struct ts_value* const op = value_from_double_matrix(matrix, 3, 3);
        char* const string = value_as_string(op);
        printf("%s", string);

        // REQUIRE(strcmp(string, "TS_STRING: test") == 0);
        free(op);
        free(string);
    } //
}