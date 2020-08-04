#NAME: Son Dang
#EMAIL: sonhdang@ucla.edu
#ID: 105215636

CC=gcc
CFLAGS=-Wall -Wextra
TARGET=lab2_add

.PHONY: tests graphs clean dist build

.DEFAULT_GOAL := build

build: lab2_add
	$(CC) $(CFLAGS) -pthread -o lab2_add lab2_add.o
#	$(CC) $(CFLAGS) -o lab2_list lab2_list.o

lab2_add.o: lab2_add.c
	$(CC) $(CFLAGS) -c lab2_add.c

#lab2_list.o: lab2_list.c
#	$(CC) $(CFLAGS) -c lab2_list.c

tests:


graphs:


clean:

#temp dist
dist:
	tar -zcvf lab2a-105215636.tar.gz lab2_add.c Makefile

#official dist
#dist:
#	tar -zcvf lab2a-105215636.tar.gz lab2_add.c SortedList.h SortedList.c lab2_list.c Makefile README lab2_add.csv lab2_list.csv lab2_add-{1..5}.png lab2_list-{1..4}.png