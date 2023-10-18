#!/bin/bash

director="$1"
caracter="$2"

for arg in "$1"/*.txt
do
    chmod +"$2" "$arg"
    echo "asd"
done

for dir in "$1"/*
do
    if test -d "dir"
       then
       bash "$0" "$dir" "$2" 
    fi
done
