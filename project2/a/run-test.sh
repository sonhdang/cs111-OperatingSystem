#! /bin/bash
# Shell script to run all the tests

file="lab2_add.csv"
if [ -f $file ]
then
    rm $file
fi

for t_yield in 2 4 8 12     # YIELD - NO SYNCHRONIZATION
do
    for i_yield in 10 20 40 80 100 1000 10000 100000
    do
        ./lab2_add --threads=$t_yield --iterations=$i_yield --yield >> lab2_add.csv
    done
done

for t in 2 8                # NO YIELD - NO SYNCHRONIZATION
do
    for i in 100 1000 10000 100000
    do
        ./lab2_add --threads=$t --iterations=$i >> lab2_add.csv
    done
done

for t_yield in 2 4 6 8 12   # YIELD - MUTEX
do
    ./lab2_add --threads=$t_yield --iterations=10000 --yield --sync=m >> lab2_add.csv
done

for t_yield in 2 4 6 8 12   # YIELD - COMPARE AND SWAP
do
    ./lab2_add --threads=$t_yield --iterations=10000 --yield --sync=c >> lab2_add.csv
done

for t_yield in 2 4 6 8 12   # YIELD - SPIN LOCK
do
    ./lab2_add --threads=$t_yield --iterations=1000 --yield --sync=s >> lab2_add.csv
done

for t in 1 2 4 8 12         # NO YIELD - MUTEX, CAS, SPINLOCK
do
    for s in 'm' 's' 'c'
    do
        ./lab2_add --threads=$t --iterations=10000 --sync=$s >> lab2_add.csv
    done
done

for t in 1 2 4 8 12         # NO YIELD - NO SYNCHRONIZATION
do
    ./lab2_add --threads=$t --iterations=10000 >> lab2_add.csv
done