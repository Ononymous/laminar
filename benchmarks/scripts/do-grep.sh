#!/bin/bash

regex_mappers=(1 1 2 4 5 8 10 20 40 50 80 100)
rm -f grep_mapreduce.txt
touch grep_mapreduce.txt
rm -f namespace.log

for num in "${regex_mappers[@]}"; do
    WPID=`ps auxww | grep -v grep | grep woofc-namespace | awk '{print $2}'`
    kill -9 $WPID
    DPID=`docker ps | grep -v "CONT" | awk '{print $1}'`
    docker kill $DPID
    rm -f ./lmr*
    ./woofc-namespace-platform >& namespace.log &
    sleep 5 &&
    ./grep_mapreduce lam -r $num >> grep_mapreduce.txt
    sleep 1
done

inv_mappers=(1 1 2 4 8 16 32)
for num in "${inv_mappers[@]}"; do
    WPID=`ps auxww | grep -v grep | grep woofc-namespace | awk '{print $2}'`
    kill -9 $WPID
    DPID=`docker ps | grep -v "CONT" | awk '{print $1}'`
    docker kill $DPID
    rm -f ./lmr*
    ./woofc-namespace-platform >& namespace.log &
    sleep 5 &&
    ./grep_mapreduce lam -r 8 -i $num >> grep_mapreduce.txt
    sleep 1
done

grep "Latency" grep_mapreduce.txt