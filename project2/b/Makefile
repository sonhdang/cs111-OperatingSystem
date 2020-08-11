#NAME: Son Dang
#EMAIL: sonhdang@ucla.edu
#ID: 105215636

CC=gcc
CFLAGS=-Wall -Wextra -g -lprofiler
CSV=lab2b_list.csv

.PHONY: default graphs clean dist build profile

default: lab2_list
	
lab2_list: lab2_list.o SortedList.o
	$(CC) $(CFLAGS) -pthread -o lab2_list lab2_list.o SortedList.o

lab2_list.o: lab2_list.c SortedList.h
	$(CC) $(CFLAGS) -c lab2_list.c

SortedList.o: SortedList.c SortedList.h
	$(CC) $(CFLAGS) -c SortedList.c

tests:
	#lab2b_1.png + lab2b_2.png
	@./lab2_list --threads=1 --iterations=1000 --sync=m > $(CSV)
	@./lab2_list --threads=2 --iterations=1000 --sync=m >> $(CSV)
	@./lab2_list --threads=4 --iterations=1000 --sync=m >> $(CSV)
	@./lab2_list --threads=8 --iterations=1000 --sync=m >> $(CSV)
	@./lab2_list --threads=12 --iterations=1000 --sync=m >> $(CSV)
	@./lab2_list --threads=16 --iterations=1000 --sync=m >> $(CSV)
	@./lab2_list --threads=24 --iterations=1000 --sync=m >> $(CSV)
	@./lab2_list --threads=1 --iterations=1000 --sync=s >> $(CSV)
	@./lab2_list --threads=2 --iterations=1000 --sync=s >> $(CSV)
	@./lab2_list --threads=4 --iterations=1000 --sync=s >> $(CSV)
	@./lab2_list --threads=8 --iterations=1000 --sync=s >> $(CSV)
	@./lab2_list --threads=12 --iterations=1000 --sync=s >> $(CSV)
	@./lab2_list --threads=16 --iterations=1000 --sync=s >> $(CSV)
	@./lab2_list --threads=24 --iterations=1000 --sync=s >> $(CSV)
	#lab2b_3.png
	-@./lab2_list --threads=1 --iterations=1 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=2 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=4 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=8 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=16 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=1 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=2 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=4 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=8 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=16 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=1 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=2 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=4 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=8 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=16 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=12 --iterations=1 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=12 --iterations=2 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=12 --iterations=4 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=12 --iterations=8 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=12 --iterations=16 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=16 --iterations=1 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=16 --iterations=2 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=16 --iterations=4 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=16 --iterations=8 --yield=id --lists=4 >> $(CSV)
	-@./lab2_list --threads=16 --iterations=16 --yield=id --lists=4 >> $(CSV)

	-@./lab2_list --threads=1 --iterations=10 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=20 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=40 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=80 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=10 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=20 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=40 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=80 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=10 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=20 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=40 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=80 --yield=id --sync=s --lists=4 >> $(CSV)
	-@./lab2_list --threads=12 --iterations=10 --yield=id --lists=4 --sync=s >> $(CSV)
	-@./lab2_list --threads=12 --iterations=20 --yield=id --lists=4 --sync=s >> $(CSV)
	-@./lab2_list --threads=12 --iterations=40 --yield=id --lists=4 --sync=s >> $(CSV)
	-@./lab2_list --threads=12 --iterations=80 --yield=id --lists=4 --sync=s >> $(CSV)
	-@./lab2_list --threads=16 --iterations=10 --yield=id --lists=4 --sync=s >> $(CSV)
	-@./lab2_list --threads=16 --iterations=20 --yield=id --lists=4 --sync=s >> $(CSV)
	-@./lab2_list --threads=16 --iterations=40 --yield=id --lists=4 --sync=s >> $(CSV)
	-@./lab2_list --threads=16 --iterations=80 --yield=id --lists=4 --sync=s >> $(CSV)

	-@./lab2_list --threads=1 --iterations=10 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=20 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=40 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=1 --iterations=80 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=10 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=20 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=40 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=4 --iterations=80 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=10 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=20 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=40 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=8 --iterations=80 --yield=id --sync=m --lists=4 >> $(CSV)
	-@./lab2_list --threads=12 --iterations=10 --yield=id --lists=4 --sync=m >> $(CSV)
	-@./lab2_list --threads=12 --iterations=20 --yield=id --lists=4 --sync=m >> $(CSV)
	-@./lab2_list --threads=12 --iterations=40 --yield=id --lists=4 --sync=m >> $(CSV)
	-@./lab2_list --threads=12 --iterations=80 --yield=id --lists=4 --sync=m >> $(CSV)
	-@./lab2_list --threads=16 --iterations=10 --yield=id --lists=4 --sync=m >> $(CSV)
	-@./lab2_list --threads=16 --iterations=20 --yield=id --lists=4 --sync=m >> $(CSV)
	-@./lab2_list --threads=16 --iterations=40 --yield=id --lists=4 --sync=m >> $(CSV)
	-@./lab2_list --threads=16 --iterations=80 --yield=id --lists=4 --sync=m >> $(CSV)

	#lab2b_4.png + lab2b_5.png
	@./lab2_list --threads=1 --iterations=1000 --sync=m --list=4 >> $(CSV)
	@./lab2_list --threads=2 --iterations=1000 --sync=m --list=4 >> $(CSV)
	@./lab2_list --threads=4 --iterations=1000 --sync=m --list=4 >> $(CSV)
	@./lab2_list --threads=8 --iterations=1000 --sync=m --list=4 >> $(CSV)
	@./lab2_list --threads=12 --iterations=1000 --sync=m --list=4 >> $(CSV)

	@./lab2_list --threads=1 --iterations=1000 --sync=m --list=8 >> $(CSV)
	@./lab2_list --threads=2 --iterations=1000 --sync=m --list=8 >> $(CSV)
	@./lab2_list --threads=4 --iterations=1000 --sync=m --list=8 >> $(CSV)
	@./lab2_list --threads=8 --iterations=1000 --sync=m --list=8 >> $(CSV)
	@./lab2_list --threads=12 --iterations=1000 --sync=m --list=8 >> $(CSV)

	@./lab2_list --threads=1 --iterations=1000 --sync=m --list=16 >> $(CSV)
	@./lab2_list --threads=2 --iterations=1000 --sync=m --list=16 >> $(CSV)
	@./lab2_list --threads=4 --iterations=1000 --sync=m --list=16 >> $(CSV)
	@./lab2_list --threads=8 --iterations=1000 --sync=m --list=16 >> $(CSV)
	@./lab2_list --threads=12 --iterations=1000 --sync=m --list=16 >> $(CSV)

	@./lab2_list --threads=1 --iterations=1000 --sync=s --list=4 >> $(CSV)
	@./lab2_list --threads=2 --iterations=1000 --sync=s --list=4 >> $(CSV)
	@./lab2_list --threads=4 --iterations=1000 --sync=s --list=4 >> $(CSV)
	@./lab2_list --threads=8 --iterations=1000 --sync=s --list=4 >> $(CSV)
	@./lab2_list --threads=12 --iterations=1000 --sync=s --list=4 >> $(CSV)

	@./lab2_list --threads=1 --iterations=1000 --sync=s --list=8 >> $(CSV)
	@./lab2_list --threads=2 --iterations=1000 --sync=s --list=8 >> $(CSV)
	@./lab2_list --threads=4 --iterations=1000 --sync=s --list=8 >> $(CSV)
	@./lab2_list --threads=8 --iterations=1000 --sync=s --list=8 >> $(CSV)
	@./lab2_list --threads=12 --iterations=1000 --sync=s --list=8 >> $(CSV)

	@./lab2_list --threads=1 --iterations=1000 --sync=s --list=16 >> $(CSV)
	@./lab2_list --threads=2 --iterations=1000 --sync=s --list=16 >> $(CSV)
	@./lab2_list --threads=4 --iterations=1000 --sync=s --list=16 >> $(CSV)
	@./lab2_list --threads=8 --iterations=1000 --sync=s --list=16 >> $(CSV)
	@./lab2_list --threads=12 --iterations=1000 --sync=s --list=16 >> $(CSV)

graphs:
	gnuplot ./lab2b.gp

profile:
	LD_PRELOAD=/usr/lib64/libprofiler.so
	CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.out
	pprof --list=thread_worker ./lab2_list ./raw.gperf >> profile.out

clean:
	rm -f *.o lab2b_105215636.tar.gz lab2_list

dist:
	tar -zcvf lab2b-105215636.tar.gz SortedList.h SortedList.c lab2_list.c Makefile lab2b_list.csv profile.out README lab2b.gp lab2b_{1..5}.png