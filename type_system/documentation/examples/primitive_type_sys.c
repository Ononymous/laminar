//
// Created by Lukas Brand on 10.06.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <type_system/ts_types.h>
#include <type_system/types/ts_primitive.h>
#include <woofc.h>

int main() {
    const char* const woof_name = "example";
    const int history_size = 100;

    WooFInit();
    WooFCreate(woof_name, TS_VALUE_SIZE, history_size);

    struct ts_value send;
    set_double(&send, 1234.5);
    WooFPut(woof_name, NULL, &send);
    struct ts_value receive;
    WooFGet(woof_name, &receive, WooFGetLatestSeqno(woof_name));

    char* const value_string = value_as_string(&receive);
    printf("Received: %s", value_string);
    free(value_string);
}