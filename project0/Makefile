#NAME: Son Dang
#ID: 105215636
#EMAIL: sonhdang@ucla.edu

CC=gcc
CFLAGS=-Wall -Wextra
TARGET=lab0

$(TARGET): $(TARGET).o
	gcc -Wall -Wextra -o $(TARGET) $(TARGET).o

$(TARGET).o: $(TARGET).c
	gcc -Wall -Wextra -c $(TARGET).c

.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET).o

.PHONY: dist
dist:
	tar -zcvf $(TARGET)-105215636.tar.gz $(TARGET).c Makefile README backtrace.png breakpoint.png