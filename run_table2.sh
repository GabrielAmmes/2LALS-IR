#!/bin/bash

for F1 in benchmarks/table2/*
do
    P1=`basename $F1`
    for er in 1 3 5
    do
        ./main -i benchmarks/table2/${P1} -o app/table2/ -er ${er} -t 0
    done
done