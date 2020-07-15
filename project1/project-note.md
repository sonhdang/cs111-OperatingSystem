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
* Data written to the write end of the pipe is buffered by the kernel until it is read from the read end of the pipe.
* Example Program: the following program creates a pipe, and the `fork(2)`s to create a child process; the child inherits a duplicate set of file descriptors that refer to the same pipe. After the `fork(2)`, each process closes the file descriptors that it doesn't need for the pipe (see `pipe(7)`). The parent then writes the string contained in the program's command-line argument to the pipe, and the child reads this string a byte at a time from the pipe and echoes it on standard output.

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
main(int argc, char *argv[])
{
   int pipefd[2];
   pid_t cpid;
   char buf;

   if (argc != 2) {
       fprintf(stderr, "Usage: %s <string>\n", argv[0]);
       exit(EXIT_FAILURE);
   }

   if (pipe(pipefd) == -1) {
       perror("pipe");
       exit(EXIT_FAILURE);
   }

   cpid = fork();
   if (cpid == -1) {
       perror("fork");
       exit(EXIT_FAILURE);
   }

   if (cpid == 0) {    /* Child reads from pipe */
       close(pipefd[1]);          /* Close unused write end */

       while (read(pipefd[0], &buf, 1) > 0)
           write(STDOUT_FILENO, &buf, 1);

       write(STDOUT_FILENO, "\n", 1);
       close(pipefd[0]);
       _exit(EXIT_SUCCESS);

   } else {            /* Parent writes argv[1] to pipe */
       close(pipefd[0]);          /* Close unused read end */
       write(pipefd[1], argv[1], strlen(argv[1]));
       close(pipefd[1]);          /* Reader will see EOF */
       wait(NULL);                /* Wait for child */
       exit(EXIT_SUCCESS);
   }
}
```

* For more, please read: <https://man7.org/linux/man-pages/man2/pipe.2.html>
* Youtube videos:
	* Pipe() tutorial for linux <https://youtu.be/uHH7nHkgZ4w>
	* C Programming in Linux Tutorial #037 - pipe() Function <https://youtu.be/7bNMkvcOKlc>

### exec()

```c
int execv(const char *pathname, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

* The `exec()` family of functions replaces the current process image with a new process image.
* The initial argument for these functions is the name of a file that is to be executed.
* Source:
	* <https://man7.org/linux/man-pages/man3/exec.3.html>
	* <https://www.youtube.com/watch?v=kDxjcyHu_Qs>

### poll()

```c
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

* `poll()` waits for one of a set of file descriptors to become ready to perform I/O.
* The set of file descriptors to be monitored is specified in the `fds` argument, which is an array of structures of the following form:

```c
struct pollfd {
	int   fd;         /* file descriptor */
	short events;     /* requested events */
	short revents;    /* returned events */
};
```

The caller should specify the number of items in the fds array in nfds.

* The field `fd` contains a file descriptor for an open file.  If this field is negative, then the corresponding `events` field is ignored and the `revents` field returns zero.  (This provides an easy way of ignor‐ing a file descriptor for a single poll() call: simply negate the fd field.  Note, however, that this technique can't be used to ignore file descriptor 0.)
* The field `events` is an input parameter, a bit mask specifying the events the application is interested in for the file descriptor `fd`. This field may be specified as zero, in which case the only events that can be returned in `revents` are `POLLHUP`, `POLLERR`, and `POLLNVAL`.
* The field `revents` is an output parameter, filled by the kernel with the events that actually occurred.  The bits returned in `revents` can include any of those specified in `events`, or one of the values `POLLERR`, `POLLHUP`, or `POLLNVAL`. (These three bits are meaningless in the `events` field, and will be set in the `revents` field whenever the corresponding condition is true.)
* If none of the events requested (and no error) has occurred for any of the file descriptors, then `poll()` blocks until one of the events occurs.
* The timeout argument specifies the number of milliseconds that poll() should block waiting for a file descriptor to become ready. The call will block until either:
	* a file descriptor becomes ready;
	* the call is interrupted by a signal handler; or
	* the timeout expires.
* Note that the timeout interval will be rounded up to the system clock granularity, and kernel scheduling delays mean that the blocking interval may overrun by a small amount.  Specifying a negative value in timeout means an infinite timeout.  Specifying a timeout of zero causes poll() to return immediately, even if no file descriptors are ready.
* Return value:
	* On success, `poll()` returns a nonnegative value which is the number of elements in the `pollfds` whose `revents` fields have been set to a nonzero value (indicating an event or an error).
	* A return value of zero indicates that the system call timed out before any file descriptors became read.
	* On error, -1 is returned, and errno is set to indicate the cause of the error.
* Source:
	* <https://youtu.be/UP6B324Qh5k>
	* <https://man7.org/linux/man-pages/man2/poll.2.html>

### dup() and dup2()

```c
int dup(int oldfd);
int dup2(int oldfd, int newfd)
```

* The `dup()` systemcall creates a copy of the file descriptor `oldfd`, using the lowest-numbered unused file descriptor for the new descriptor
* The `dup2()` system call performs the same task as `dup()`, but instead of using the lowest-numbered unused file descriptor, it uses the file descriptor number specified in `newfd`. If the file descriptor `newfd` was previously open, it is silently closed before being reused.
* Source:
	* <https://man7.org/linux/man-pages/man2/dup.2.html>
	* <https://www.youtube.com/watch?v=EqndHT606Tw

### kill()

```c
int kill(pid_t pid, int sig);
```

### signal()

* Source:
	* <https://youtu.be/rggw61JtGz0>

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

## Understanding the projects
* Instead of constructing a set of correct terminal modes from scracth, it was suggested that we make a copy of the  current terminal and change the flags to:

```c
	c_iflag = ISTRIP;
	c_oflag = 0;
	c_lflag = 0;
```

* Understanding canonical and non-canonical mode.
	* Canonical mode: What you type are visible on the STDOUT_FILENO (usually the screen) and you can edit what you type with *backspace* and *kill* **characters**.
		* Note that *backspace* and *kill* are **characters** which affects what the programs read in.
		* Even though all the characters are visible on the terminal, it was not read until it reaches the *return* character which signal the end of line.
		* Only after then does the program execute `read()` and if there is a `write()` function call, the text will then be displayed after being processed with the current **termios mode**.
		* An example of canonical mode is the command `cat`. When you execute the command `cat`, its input file is `STDIN` and its output file is `STDOUT`. `cat` read lines by line. Therefore when you type characters out, `cat` shows you what you have entered and erase characters when you use *backspace*. It's not until you pressed the *return* key does cat start to read in the text and write to `STDOUT`
	* Non-canonical mode: When a character is pressed, it is immediately read in the program. It does not wait for the *return* key to start process the text. Therefore, if you have a text "asdf\BackSpace", it will be read in "a" then "s" then "d" then "f" then "\BackSpace" instead of "asdf\Backspace" all at the same time.
	* For more, please read: <https://stackoverflow.com/questions/358342/canonical-vs-non-canonical-terminal-input>

## Problems encountered

#### Cannot figure out a way to map <cr> or <lf> into <cr><lf>.

* It actually turned out that because the terminal is in non-canonical mode, I need to process individual character read in.
* Therefore I just need to compare if the character in the buffer is equal to the carriage return character '\r'  and write out '\r' with '\n' instead.

#### Cannot send EOF signal to the terminal with ctrl-D

* The reason for this problem was pressing ctrl-D is read as **EOT**, not the macro **EOF** in `<stdio.h>`.
* I didn't know that was the case so I kept comparing the character with **EOF**.
* I figured out after trying to print out all the characters in decimal instead of char (because there is no representation for **EOT** in char)
* I got number 4 for printing out **EOT**.
* Then I look it up on the ASCII table and find out it is **EOT**.
* I make a `const char EOT = 0x04;` and compare the character in the buffer with `EOT`.

#### Should the parent process still `wait()` after the child process call `exec()`?

* In the case where there is only `fork()`, the parent should wait for the child process to terminate and return to ensure that the child process terminate correctly, before the parent process terminate.
* Answer from stackoverflow: "You need to distinguish the **process** from the **program**. Calling `exec` runs a different program in the same process. The `exec` function doesn't return (except to signal an error) because it terminates the program that calls in. However, the process is reused to run a different program. In a way, from the perspective of the process running `exec`, the `exec` functions returns as the entry point of the new program. From the point of view of the parent, there's still a child process. That's all the parent knows. The parent doesn't know that the child called `exec`, unless it watches it and finds out by indirect means such as running `ps`. The parent is just waiting for the child process to exit, no matter what program the child proccess happens to be running."
* Source: <https://stackoverflow.com/questions/35710102/if-you-fork-and-exec-from-the-child-process-and-wait-in-the-parent-how-doe>

## Miscelaneous

#### MACRO

* Carriage Return (CR) means going to the left most of the line
* New Line (NL) means going down one line.
* When press enter on the keyboard, the behavior triggered by the enter key is determined by whether our terminal interpret it as a CR, NL or CR-NL.
* For `c_iflag`:
	* INLCR: translate NL to CR on input
	* ICRNL: translate CR to NL on input
* For `c_oflag`:
	* ONCLR: Map NL to CR-NL on output
	* OCRNL: Map CR to NL on ouput