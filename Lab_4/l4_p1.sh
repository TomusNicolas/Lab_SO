#!/bin/bash


while read line
do
    if [ "$(echo $line | grep -E "^[A-Z][a-zA-Z0-9\ ,]+\.{1}$" | grep -E -v "si[\ ]*\," | grep -E -v "n[pb]" )" ]
    then echo "ok"
    else echo "not ok"
    fi
done
