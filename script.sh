#!/bin/bash

if test "$#" -gt 3
then
    echo "Are numarul corect de argumente"
else
    echo "Nu are numarul corect de argumente"
fi

count=0
sum=0
file=$1
dir=$2
shift 2

for arg in "$@"
do
    if [ $arg -gt 5 ]
    then
	count=`expr $count + 1`
    fi
    sum=`expr $sum + $arg`
done
echo $count
echo $sum

echo $sum | wc -L

if test -f "$file.txt"
then
    echo "asd"
else
    echo "nu"
fi

