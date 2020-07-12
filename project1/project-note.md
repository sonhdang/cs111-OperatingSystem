# PROJECT NOTES

## Terminology

### What is a *multi-process telnet-like* client and server

* According to wikipedia,
    * "*Multiprocessing* is the use of two or more central processing units (CPUs) within a single computer system. The term also refers to the ability of a system to support more than one processor or the ability to allocate tasks between them."
    * "At the operating system level, *multiprocessing* is sometimes used to refer to the execution of multiple concurrent *processes* in a system, with each process running on a separate CPU or core, as oppsed to a single process at any one instant."
    * When used with this definition, *multiprocessing* is sometimes contrasted with *multitasking*, which may use just a single processor but switch it in times slices between tasks (i.e. a time-sharing system)
* *Telnet* is an application protocol used on the internet or local are network to provide a bidirectional interactive text-oriented communication facility using a virtual terminal connection.
    * Historically, *Telnet* provided access to a command-line interface on a remote host. However, because of serious security concerns when using Telnet over an open network such as the Internet, its use for this purpose has waned significantly in favor of *SSH*.

### What is *duplex*

* There are three modes of transmission, namely: simplex, half duplex, and full duplex. The transmission mode defines the direction of signal flow between two connected devices
* The primary difference between three modes of transmission is that:
    * In a simple mode of transmission the communication is unidirectional, or one-way.
    * In the half duplex mode of transmission the communication is two-directional, but the channel is interchangeably used by both of the connected devices (not at the same time).
    * In the full duplex mode of transmission, the communication is bi-directional or two-way, and the channel is used by both of the connecte4d devices simultaneously (at the same time).

### What is *inter-process communication*

* Inter-process communication is used for exchanging data between multiple threads in one or more processes or programs. The Processes may be running on single or multiple computers connected by a network.
* It is a set of programming interface which allow a programmer to coordinate activities among various program processes which can run concurrently in an operating system. This allow a specific program to handle many user requests at the same time.
* Approaches for Inter-Processes Communication
	* Pipes: pipe is widely used for communication between two related processes. This is a half-duplex method, so the first process communicates with the second process. However, in order to achieve a full-duplex, another pipe is needed.
	* Message Queues: a message queue is a linked list of messages stored within the kernel. It is identified by a message queue identifier. This method offeres communication between single or multiple processes with full-duplex capacity.
	* Direct Communication: In this type of inter-process communication process, should name each other explicitly. In this method, a link is established between one pair of communicating processes, and between each pair, only one link exists.
	* Indirect communication establishes like only when processes share a common mailbox each pair of processes sharing several communication links. A link can communicate with many processes. The link may be bi-directional or unidirectional
	* Shared memory: Share memory is a memory shared between two or more processes that are established using shared memory between all the processes. This type of memory is required to protect from each other by synchronizing access across all the processes
	* FIFO: Communication between two unrelated processes. It is a full-duplex method, which means that the first process can communicate with the second process, and the opposite can also happen.

### What is `termios`

## API

### fork()

```c
pid_t fork(void);
```

* Creates a new process by duplicating the calling process.
* The new process is referred to as the *child* process. The calling process is referred to as the *parent* process.
* The child process is an exact duplicate of the parent process except for the following points:
	* The child has its own unique process ID, and this PID does not match the ID of any existing process group or session.
	* The child's parent process ID is the same as the parent's process ID.
* For more, please read: <https://www.man7.org/linux/man-pages/man2/fork.2.html>
* Youtube videos:
	* Creating new processes with fork()! <https://youtu.be/ss1-REMJ9GA>
	* Fork() system call tutorial <https://youtu.be/xVSPv-9x3gk>

### pipe()

```c
int pipe(int pipefd[2]);
# or
int pipe2(int pipefd[2], int flags);
```

* Creates a pipe, a unidirectional data channel that can be used for interprocess communication.
* The array `pipefd` is used to return two file descriptors referring to the ends of the pipe
	* `pipefd[0]` refers to the read end of the pipe
	* `pipefd[1]` refers to the write end of the pipe.
* Data written to the write end of the pipe is buffered by the kernel until it is read from teh read end of the pipe.
* For more, please read: <https://man7.org/linux/man-pages/man2/pipe.2.html>
* Youtube videos:
	* Pipe() tutorial for linux <https://youtu.be/uHH7nHkgZ4w>
	* C Programming in Linux Tutorial #037 - pipe() Function <https://youtu.be/7bNMkvcOKlc>

### signal()

### termios
* The termios functions describe a general terminal interface that is provided to control asynchronous communications ports. Those functions include:

```c
int tcgetattr(int fd, struct termios *termios_p)
int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);
int tcsendbreak(int fd, int duration);
int tcdrain(int fd);
int tcflush(int fd, int queue_selector);
int tcflow(int fd, int action);		
```
* For more, please read: <https://man7.org/linux/man-pages/man3/termios.3.html>

## Project Summary
* Put the keyboard (the file open on file descriptor 0) into character-at-a-time, no-echo mode.
* Do not construct a set of correct terminal modes from scratch.
* Get the current terminal modes, save them for restoration, and then make a copy with only the following changes (with the `TCSANOW` option):
	* c_iflag = ISTRIP;
	* c_oflag = 0;
	* c_lflag = 0;