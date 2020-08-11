#! /usr/bin/gnuplot
#NAME: Son Dang
#EMAIL: sonhdang@ucla.edu
#ID: 105215636
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#    8. wait time per lock (ns)
#
# output:
#    lab2b_1.png ... throughput vs. number of threads for mutex and spin-lock synchronized list operations.
#    lab2b_2.png ... mean time per mutex wait and mean time per operation for mutex-synchronized list operations.
#    lab2b_3.png ... successful iterations vs. threads for each synchronization method.
#    lab2b_4.png ... throughput vs. number of threads for mutex synchronized partitioned lists.
#    lab2b_5.png ... throughput vs. number of threads for spin-lock-synchronized partitioned lists.
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","

# lab2b_1.png
set title "Throughput vs. Number of Threads"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Number of operations per second"
set logscale y 10
set output 'lab2b_1.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title 'list w/mutex' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title 'list w/spin-lock' with linespoints lc rgb 'green'


# lab2b_2.png
set title "Wait-for-mutex time wait and Time per operation for Mutex-synchronized list"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Time in nanosecond (ns)"
set logscale y 10
set output 'lab2b_2.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
	title 'average wait-for-mutex time' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
	title 'average time per operation' with linespoints lc rgb 'green'

# lab2b_3.png
set xlabel "Successful iterations"
set title "Successful iterations vs. Threads for each synchronization method"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Time"
set logscale y 10
set output 'lab2b_3.png'
set key left top
plot \
     "< grep -e 'list-id-s,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	title 'list w/spin-lock' with points lc rgb 'green', \
     "< grep -e 'list-id-m,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	title 'list w/mutex' with points lc rgb 'blue', \
     "< grep -e 'list-id-none,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	title 'unprotected' with points lc rgb 'red'

# lab2b_4.png
set title "Throughput vs. Number of threads for mutex synchronized partitioned lists"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Number of operations per second"
set logscale y 10
set output 'lab2b_4.png'
set key left bottom
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title '1 list' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title '4 lists' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title '8 lists' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title '16 lists' with linespoints lc rgb 'yellow'

# lab2b_5.png
set title "Throughput vs. Number of threads for spin-lock synchronized partitioned lists"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Number of operations per second"
set logscale y 10
set output 'lab2b_5.png'
set key left bottom
plot \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title '1 list' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title '4 lists' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title '8 lists' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):((1000000000)/($7)) \
	title '16 lists' with linespoints lc rgb 'yellow'