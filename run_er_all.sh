#!/bin/bash

for F1 in benchmarks/iwls/*
do
    P1=`basename $F1 .pla`
    for er in 1 3 5
    do
        ./main -i benchmarks/iwls/${P1}.pla -o app/all/ -er ${er} -t 0
    done
done