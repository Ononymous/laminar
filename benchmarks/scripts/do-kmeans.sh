#!/bin/bash

########## SCALING ##########

# Keep the number of points per worker same (50k) and increase number of workers

rm -f kmeans_mapreduce.txt
touch kmeans_mapreduce.txt
rm -f namespace.log

mappers=(1 1 2 4 8 16 24 32 40 48 56 64 72 80 88 96)
for num in "${mappers[@]}"; do
    WPID=`ps auxww | grep -v grep | grep woofc-namespace | awk '{print $2}'`
    kill -9 $WPID
    DPID=`docker ps | grep -v "CONT" | awk '{print $1}'`
    docker kill $DPID
    rm -f ./lmr*
    ./woofc-namespace-platform >& namespace.log &
    sleep 5 &&
    ./k_means_mapreduce -m $num >> kmeans_mapreduce.txt
    rm -rf tmp_data
    sleep 1
done

echo "========================"
echo "Linear Scaling"
echo "------------------------"
grep "AVERAGE" kmeans_mapreduce.txt
echo "========================"
rm -rf lmr*

########## SPEEDUP ##########

# Keep the total number of data points constant (1M) and increase the number of workers

rm -f kmeans_mapreduce.txt
touch kmeans_mapreduce.txt
rm -f namespace.log

mappers=(1 2 4 8 10 20 40 50 100)
for num in "${mappers[@]}"; do
    let pts_per_mapper=1000000/$num
    WPID=`ps auxww | grep -v grep | grep woofc-namespace | awk '{print $2}'`
    kill -9 $WPID
    DPID=`docker ps | grep -v "CONT" | awk '{print $1}'`
    docker kill $DPID
    rm -f ./lmr*
    ./woofc-namespace-platform >& namespace.log &
    sleep 5 &&
    ./k_means_mapreduce -m $num -p $pts_per_mapper >> kmeans_mapreduce.txt
    rm -rf tmp_data
    sleep 1
done

echo "========================"
echo "Speedup"
echo "------------------------"
grep "AVERAGE" kmeans_mapreduce.txt
echo "========================"
rm -rf lmr*
