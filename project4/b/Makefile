#NAME: Son Dang
#EMAIL: sonhdang@ucla.edu
#ID: 105215636

CC=gcc
CFLAGS=-Wall -Wextra -lmraa -lm
TARGET=lab4b

.PHONY: check clean dist

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

check:


clean:
	rm -f $(TARGET) $(TARGET).o lab4b-105215636.tar.gz

dist:
	tar -zcvf lab4b-105215636.tar.gz lab4b.c Makefile README
