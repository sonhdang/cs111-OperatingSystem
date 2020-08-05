#! /bin/bash
# Shell script to run all the tests for lab2_add

# NAME: Son Dang
# EMAIL: sonhdang@ucla.edu
# ID: 105215636

file="lab2_list.csv"
if [ -f $file ]
then
    rm $file
fi

for i in 10 100 1000 10000 20000
do
    ./lab2_list --iterations=$i >> $file
done

for t in 2 4 8 12
do
    for i in 1 10 100 1000
    do
        ./lab2_list --threads=$t --iterations=$i >> $file
    done
done

for y in 'i' 'd' "il" "dl"
do
    for t in 2 4 8 12
    do
        for i in 1 2 4 8 16 32
        do
            ./lab2_list --threads=$t --iterations=$i --yield=$y >> $file
        done
    done
done

for y in 'i' 'd' "il" "dl"
do
    for s in 'm' 's'
    do
        ./lab2_list --threads=12 --iterations=32 --yield=$y --sync=$s >> $file
    done
done

for t in 1 2 4 8 12 16 24
do
    for s in 'm' 's'
    do
        ./lab2_list --threads=$t --iterations=1000 --sync=$s >> $file
    done
done