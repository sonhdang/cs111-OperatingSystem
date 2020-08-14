#NAME: Son Dang
#EMAIL: sonhdang@ucla.edu
#ID: 105215636

CC=gcc
CFLAGS=-Wall -Wextra
TARGET=lab4a

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET).o

.PHONY: dist
dist:
	tar -zcvf $(TARGET)-105215636.tar.gz my_beaglebone.jpg connected.png copy.png build_run.png git.png Makefile README 