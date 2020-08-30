#NAME: Son Dang
#EMAIL: sonhdang@ucla.edu
#ID: 105215636

CC=gcc
CFLAGS=-Wall -Wextra
TARGET=lab3a

.PHONY: default clean dist

default: $(TARGET)
	
$(TARGET): $(TARGET).o ext2_fs.h
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

clean:
	rm -f *.o lab3a_105215636.tar.gz $(TARGET)

dist:
	tar -zcvf lab3a-105215636.tar.gz README Makefile lab3a.c ext2_fs.h