#! /bin/bash
# Shell script to run all the tests for lab2_add

# NAME: Son Dang
# EMAIL: sonhdang@ucla.edu
# ID: 105215636

file="lab2_add.csv"
if [ -f $file ]
then
    rm $file
fi

for i in 100, 1000, 10000, 100000
do
    ./lab2_add --iterations=$i >> $file
done

for t_yield in 2 4 8 12     # YIELD - NO SYNCHRONIZATION
do
    for i_yield in 10 20 40 80 100 1000 10000 100000
    do
        ./lab2_add --threads=$t_yield --iterations=$i_yield --yield >> $file
    done
done

for t in 2 8                # NO YIELD - NO SYNCHRONIZATION
do
    for i in 100 1000 10000 100000
    do
        ./lab2_add --threads=$t --iterations=$i >> $file
    done
done

for t_yield in 2 4 6 8 12   # YIELD - MUTEX
do
    ./lab2_add --threads=$t_yield --iterations=10000 --yield --sync=m >> $file
done

for t_yield in 2 4 6 8 12   # YIELD - COMPARE AND SWAP
do
    ./lab2_add --threads=$t_yield --iterations=10000 --yield --sync=c >> $file
done

for t_yield in 2 4 6 8 12   # YIELD - SPIN LOCK
do
    ./lab2_add --threads=$t_yield --iterations=1000 --yield --sync=s >> $file
done

for t in 1 2 4 8 12         # NO YIELD - MUTEX, CAS, SPINLOCK
do
    for s in 'm' 's' 'c'
    do
        ./lab2_add --threads=$t --iterations=10000 --sync=$s >> $file
    done
done

for t in 1 2 4 8 12         # NO YIELD - UNPROTECTED
do
    ./lab2_add --threads=$t --iterations=10000 >> $file
done