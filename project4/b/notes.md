## poll(2)

```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

#define _GNU_SOURCE
#include <signal.h>
#include <poll.h>

int ppoll(struct pollfd *fds, nfds_t nfds,
		const struct timespec *tmo_p, const sigset_t *sigmask);
```
The `poll(2)` is a **system call**, used to wait for some even on file descriptor. It performs a similar task to `select(2)` waits for one of a set of file descriptors to become ready to perform I/O.

A typical situation where `poll()` is useful is when a program wants to process inputs from `STDIN` but still wants to do other things while waiting for the input from `STDIN`. Since `read()` is a **blocking** routine, your program might be halted while waiting for the user input. `poll()` allows the program to initiate `read()` routine for a certain amount of time before it **timeouts** and **returns** so that other routines can have a chance to run.

Source:

* <https://man7.org/linux/man-pages/man2/poll.2.html>
* <https://youtu.be/UP6B324Qh5k>

# kill(2)

```c
#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);
```

The `kill(2)` system call is used to send signal to a specified process or a process group.

* If pid is positive, then signal sig is sent to the process with the ID specified by pid.
* If pid equals 0, then sig is sent to every process in the process group of the calling process.
* If pid equals -1, then sig is sent to every process for which the calling process has permission to send signals, except for process 1 (init), but see below.
* If pid is less than -1, then sig is sent to every process in the process group whose ID is -pid.
* If sig is 0, then no signal is sent, but existence and permission checks are still performed; this can be used to check for the existence of a process ID or process group ID that the caller is permitted to signal.

Source:

* <https://man7.org/linux/man-pages/man2/kill.2.html>

# read(2)

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```

`read()` attempts to read up to `count` bytes from file descriptor `fd` into the buffer starting at `buf`.

On files that support seeking, the read operation commences at the file offset, and the file offset is incremented by the number of bytes read. If the file offset is at or past the end of file, no bytes are read, and `read()` returns zero.

If `count` is zero, `read()` may detect the errors described below. In the absence of any errors, or if `read()` does not check for errors, a `read()` with a `count` of 0 returns zero and has no other effects.

<https://man7.org/linux/man-pages/man2/read.2.html>

# strcpy()

```c
#include <string.h>

char* strcpy(char* destination, const char* source);
```

* The `strcpy()` function copies the string pointed by `source` (**including the null character**) to the `destination`.
* The `strcpy()` function also returns the copied string.

<https://www.programiz.com/c-programming/library-function/string.h/strcpy>

# Initializing a char array/string in C

There are several ways

```c
char buf[10] = "";
memset(buf, 0, 10);

```

"`memset()` is overkill and just zeroing the first element suffices (as noted in other answers)."

<https://stackoverflow.com/questions/1559487/how-to-empty-a-char-array>

# Getting a substring in C

Either use `memcpy()` or `strncpy()`

```c
char subbuff[5];
memcpy( subbuff, &buff[10], 4 );
subbuff[4] = '\0';
```

<https://stackoverflow.com/questions/4214314/get-a-substring-of-a-char>

# Miscelaneous

## How to create a file if it does not exist in the current directory or overwrite it if it already exists?

Add the permission flag as the third argument of **system call** `open()`

```c
int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 644);
```

<https://stackoverflow.com/questions/23092040/how-to-open-a-file-which-overwrite-existing-content>

## How to create a string with format specifier?

Use `sprintf()`

```c
char buf [50];
int n, a=5, b=3;
n = sprintf (buf, "%d plus %d is %d", a, b, a+b);
printf ("[%s] is a string %d chars long\n", buf, n);
```

<https://stackoverflow.com/questions/4357927/create-a-string-in-c-with-a-format-specifier>
<http://www.cplusplus.com/reference/cstdio/sprintf/>

## How to check if a substring exists in a string in shell scripting?

```bash
string='My long string'
if [[ $string == *"My long"* ]]; then
  echo "It's there!"
fi
```

<https://stackoverflow.com/questions/229551/how-to-check-if-a-string-contains-a-substring-in-bash>
<https://linuxize.com/post/how-to-check-if-string-contains-substring-in-bash/>