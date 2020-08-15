#NAME: Son Dang
#EMAIL: sonhdang@ucla.edu
#ID: 105215636

CC=gcc
CFLAGS=-Wall -Wextra -lmraa -lm
TARGET=lab4b

.PHONY: check clean dist

$(TARGET): $(TARGET).c
	./compile.sh

check:
	{ echo "PERIOD = 2"; sleep 3; echo "STOP"; sleep 2; echo "OFF"; } | ./lab4b --scale=C --log=output.txt
	rm output.txt

clean:
	rm -f $(TARGET) lab4b-105215636.tar.gz compile.sh

dist:
	tar -zcvf lab4b-105215636.tar.gz lab4b.c Makefile README compile.sh
