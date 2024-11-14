#!/bin/bash

rm -f ./rrrttt
rm -f ./riot-etl.txt
CNT=1
while ( test $CNT -lt 100 ) ; do
	WPID=`ps auxww | grep -v grep | grep woofc-namespace | awk '{print $2}'`
	kill -9 $WPID
	DPID=`docker ps | grep -v "CONT" | awk '{print $1}'`
	docker kill $DPID
	./woofc-namespace-platform >& namespace.log &
	./riot_benchmark >> .riot-etl.txt
	ST=`grep "RIOT" namespace.log | grep "START" | awk '{print $4}'`
	EN=`grep "RIOT" namespace.log | grep "STOP" | awk '{print $4}'`
	DUR=`echo $ST $EN | awk '{print ($2-$1)*1000.0}'`
	echo $DUR
	CNT=$(($CNT+1))
	sleep 1
done


