#!/bin/bash


director="$1"
nume="$2"

count=0
sum=0

for fisier in "$1"/*.txt
do
    count=$(wc -m < "$fisier")
    sum=`expr $sum + $count`
    echo "$fisier $count" >>  "$nume"
done
echo "Total $sum" >> "$nume"
