#!/bin/bash
cd data
MAXVAL=$1
for ((i = 0; i <= MAXVAL; ++i))
do
    rm $i
done
cd ..
