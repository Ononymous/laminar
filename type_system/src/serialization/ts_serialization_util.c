//
// Created by Lukas Brand on 03.07.2023.
//
#include <stdbool.h>
#include <stdint.h>
#include <woofc.h>

bool woof_exists(const char* woof_id) {
    if (WooFInvalid(WooFGetLatestSeqno(woof_id))) {
        return false;
    } else {
        return true;
    }
}

void unsigned_long_to_byte_array(unsigned long value, uint8_t* array) {
    int i;
    for (i = 0; i < 8; i++) {
        array[i] = (value >> (i * 8)) & 0xFF;
    }
}

unsigned long byte_array_to_unsigned_long(const uint8_t* const array) {
    unsigned long value = 0;
    int i;
    for (i = 0; i < 8; i++) {
        value |= (unsigned long)array[i] << (i * 8);
    }
    return value;
}