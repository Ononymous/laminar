//
// Created by Lukas Brand on 10.06.2023.
//
#include <stdio.h>
#include <woofc.h>

int main() {
    const char* const woof_name = "example";
    const int history_size = 100;

    WooFInit();
    WooFCreate(woof_name, sizeof(double), history_size);

    double send = 1234.5;
    WooFPut(woof_name, NULL, &send);
    double receive = -1;
    WooFGet(woof_name, &receive, WooFGetLatestSeqno(woof_name));

    printf("Received: %f", receive);
}