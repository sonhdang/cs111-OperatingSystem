#NAME: Son Dang
#EMAIL: sonhdang@ucla.edu
#ID: 105215636

CC=gcc
CFLAGS=-Wall -Wextra
TARGET=lab2_add

.PHONY: tests graphs clean dist build

.DEFAULT_GOAL := build

build: lab2_add lab2_list
	
lab2_add: lab2_add.o
	$(CC) $(CFLAGS) -pthread -o lab2_add lab2_add.o

lab2_list: lab2_list.o SortedList.o
	$(CC) $(CFLAGS) -pthread -o lab2_list lab2_list.o SortedList.o

lab2_add.o: lab2_add.c
	$(CC) $(CFLAGS) -c lab2_add.c

lab2_list.o: lab2_list.c SortedList.h
	$(CC) $(CFLAGS) -c lab2_list.c

SortedList.o: SortedList.c SortedList.h
	$(CC) $(CFLAGS) -c SortedList.c

tests:
	./test-add.sh
	./test-list.sh

graphs:
	gnuplot ./lab2_add.gp
	gnuplot ./lab2_list.gp

clean:
	rm -f lab2_add.o lab2_list.o SortedList.o

dist:
	tar -zcvf lab2a-105215636.tar.gz lab2_add.c SortedList.h SortedList.c lab2_list.c Makefile README lab2_add.csv lab2_list.csv lab2_add-{1..5}.png lab2_list-{1..4}.png test-add.sh test-list.sh