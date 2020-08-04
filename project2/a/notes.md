# The Pthreads API

The subroutines which comprise the Pthreads API can be informally grouped into four major groups:

1. **Thread management**: Routines that work directly on threads - creating, detaching, joining, etc. They also include functions to set/query thread attributes (joinable, scheduling etc.)
2. **Mutexes**: Routines that deal with synchronization, called a "mutex", which is an abbreviation for "mutual exclusion". Mutex functions provide for creating, destroying, locking and unlocking mutexes. These are supplemented by mutex attribute functions that set or modify attributes associated with mutexes.
3. **Condition variables**: Routines that address communications between threads that share a mutex. Based upon programmer specified conditions. This group includes functions to create, destroy, wait and signal based upon specified variable values. 4. Functions to set/query condition variable attributes are also included.
4. **Synchronization**: Routines that manage read/write locks and barriers.

# Threads independent resources

This independent flow of control is accomplished because a thread maintains its own:

* Stack pointer
* Registers
* Scheduling properties (such as policy or priority)
* Set of pending and blocked signals
* Thread specific data.

# Things to note when using threads

Because threads within the same process share resources:

* Changes made by one thread to shared system resources (such as closing a file) will be seen by all other threads.
* Two pointers having the same value point to the same data.
* Reading and writing to the same memory locations is possible, and therefore requires explicit synchronization by the programmer.

# Use of Pthread

* The primary motivation for considering the use of Pthreads in a high performance computing environment is to achieve optimum performance. In particular, if an application is using MPI for on-node communications, there is a potential that performance could be improved by using Pthreads instead.
* MPI libraries usually implement on-node task communication via shared memory, which involves at least one memory copy operation (process to process).
* For Pthreads there is no intermediate memory copy required because threads share the same address space within a single process. There is no data transfer, per se. It can be as efficient as simply passing a pointer.
* In the worst case scenario, Pthread communications become more of a cache-to-CPU or memory-to-CPU bandwidth issue. These speeds are much higher than MPI shared memory communications.
* Threaded applications offer potential performance gains and practical advantages over non-threaded applications in several other ways:
	* Overlapping CPU work with I/O: For example, a program may have sections where it is performing a long I/O operation. While one thread is waiting for an I/O system call to complete, CPU intensive work can be performed by other threads.
	* Priority/real-time scheduling: tasks which are more important can be scheduled to supersede or interrupt lower priority tasks.
	* Asynchronous event handling: tasks which service events of indeterminate frequency and duration can be interleaved. For example, a web server can both transfer data from previous requests and manage the arrival of new requests.

# Common models for threaded programs exist:

* ***Manager/worker***: a single thread, the *manager* assigns work to other threads, the *workers*. Typically, the manager handles all input and parcels out work to the other tasks. At least two forms of the manager/worker model are common: static worker pool and dynamic worker pool.
* ***Pipeline***: a task is broken into a series of suboperations, each of which is handled in series, but concurrently, by a different thread. An automobile assembly line best describes this model.
* ***Peer***: similar to the manager/worker model, but after the main thread creates other threads, it participates in the work.

# TLDR

So, in summary, in the UNIX environment a thread:

* Exists within a process and uses the process resources
* Has its own independent flow of control as long as its parent process exists and the OS supports it
* Duplicates only the essential resources it needs to be independently schedulable
* May share the process resources with other threads that act equally independently (and dependently)
* Dies if the parent process dies - or something similar
* Is "lightweight" because most of the overhead has already been accomplished through the creation of its process.

# Miscelaneous

* Threads have different call stacks but they're in the same memory space, which means one thread can write into the memory of another thread which makes it easier to communicate between threads. 
* Processes contain information about program resources and program execution state, including: process ID, process group ID, user ID, and group ID, environment, working directory, program instructions, registers, stack, heap, file descriptors, signal actions, shared libraries, inter-process communication tools (such as message queues, pipes, semaphores, or shared memory)
* Threads use and exist within these process resources, yet are able to be scheduled by the operating system and run as independent entities largely because they *duplicate only the bare essential resources* that enable them to exist as executable code.
* Although the Pthreads API is an ANSI/IEEE standard, implementations can, and usually do, vary in ways not specified by the standard. Because of this, a program that runs fine on one platform, may fail or produce wrong results on another platform. For example, the maximum number of threads permitted, and the default thread stack size are two important limits to consider when designing your program.

# Glossary

* MPI - Message Passing Interface: <https://en.wikipedia.org/wiki/Message_Passing_Interface>
* Node: <https://en.wikipedia.org/wiki/Node_(networking)>
