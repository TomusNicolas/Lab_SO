#!/bin/bash

while read line
do
  if [ "$(echo $line | grep -E "^[0-255]\.[0-255]\.[0-255]\.[0-255]$" )" ]
    then echo "ok"
    else echo "not ok"
  fi
done
