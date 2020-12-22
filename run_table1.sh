#!/bin/bash

for F1 in benchmarks/iwls/*
do
    P1=`basename $F1`
    ./main -i benchmarks/iwls/${P1} -o app/table1/ -en 16 -t 0
done