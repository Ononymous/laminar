#ifndef RIOT_CONFIG_H
#define RIOT_CONFIG_H

#define TIMESTAMP_FIELD "TS"    // change this if the SenML messages already have a 'TS' field

#define WINDOW_SIZE 10  // window size for average and regression

#define BUCKET_PARAM 10 // bucket param for distinct count

#define PLOT_ACCUMULATE_SIZE 20 // number of tuples accumulated before making a plot

#define STATS_FIELD "acc_chest_x"   // Field for which STATS pipeline is being run

#define DATA_PATH "/home/centos/laminar/"   // WooF namespace path

#endif
