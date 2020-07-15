//NAME: Son Dang
//EMAIL: sonhdang@ucla.edu
//ID: 105215636

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <poll.h>
#include <sys/types.h>
#include <signal.h>

/* Saving the current terminal while setting up new terminal */
void setTermios(struct termios* current)
{
    struct termios *modified = malloc(sizeof(*modified));
    if (tcgetattr(STDIN_FILENO, current) == -1)     /* tcgetattr rror Check */
    {
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    *modified = *current;
    modified->c_iflag = ISTRIP;
    modified->c_oflag = 0;
    modified->c_lflag = 0;
    tcflush(STDIN_FILENO, TCIFLUSH);
    if (tcsetattr(STDIN_FILENO, TCSANOW, modified)) /* tcsetattr error Check */
    {
        fprintf(stderr, "%s", strerror(errno));
        exit(1);
    }
}

void handler(int num) {
    char buffer[50];
    sprintf(buffer, "SHELL EXIT SIGNAL=%d STATUS=%d", num & 0x007f, num & 0xff00);
    write(STDERR_FILENO, buffer, 50);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    /* SET UP */
    // Allocating memory for temios structs
    struct termios *original = malloc(sizeof(*original));
    if (original == NULL)   /* malloc error Check */
    {
        fprintf(stderr, "%s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    // Setting buffer size to 5 to account for buffering
    char buf[5];
    char message[256];
    int byteCount_buf = 0;
    int byteCount_message = 0;
    // Constants
    const char CARRIAGE[] = {'\r'};
    const char LINEFEED[] = {'\n'};
    const char NEWLINE[] = {'\r','\n'};
    const char EOT = 0x04;
    const char INTERRUPT = 0x03;
    // Checking whether getopt_long_only performs succesfully
    int c;
    // pipefd_write for the parent to write to the child
    int pipefd_write[2];
    // pipefd_read for the parent to read from the child
    int pipefd_read[2];
    // Variables for poll()
    int p;
    // Process ID for the fork() function call
    int pid;

    setTermios(original);

    /* OPTION --shell=program */
    while(1)
    {
        static struct option long_options[] = 
        {
            {"shell", required_argument, 0, 's'},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        c = getopt_long_only(argc, argv, "", long_options, &option_index);

        /* Detect the end of the option */
        if (c == -1)
            break;
        
        switch (c)
        {
            case 's':
                signal(SIGPIPE, handler);
                /* pipe error check */
                if (pipe(pipefd_write) == -1 || pipe(pipefd_read) == -1) 
                {
                    fprintf(stderr, "%s", strerror(errno));
                    exit(EXIT_FAILURE);
                }

                
                
                pid = fork();
                if(pid == -1)       /* fork error Check */
                {
                    fprintf(stderr, "%s", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                
                else if (pid == 0)   /* Child Process */
                {
                    /* Closing unused end of pipes */
                    close(pipefd_write[1]);
                    close(pipefd_read[0]);

                    /* Redirecting STDIN, STDOU and STDERR */
                    close(STDIN_FILENO);
                    dup(pipefd_write[0]);
                    close(pipefd_write[0]);

                    close(STDOUT_FILENO);
                    dup(pipefd_read[1]);

                    close(STDERR_FILENO);
                    dup(pipefd_read[1]);
                    close(pipefd_read[1]);
                    
                    /* Replacing the child process with a shell */
                    if(execlp(optarg, optarg, NULL) == -1)    /* exec error Check */
                    {
                        fprintf(stderr, "%s", strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                }
                else            /* Parent Procss */
                {
                    /* Closing unused end of pipes */
                    close(pipefd_write[0]);
                    close(pipefd_read[1]);

                    struct pollfd pollfds[2];
                    /* STDIN POLL */
                    pollfds[0].fd = STDIN_FILENO;
                    pollfds[0].events = 0;
                    pollfds[0].events |= POLLIN | POLLHUP | POLLERR;

                    /* STDOUT POLL */
                    pollfds[1].fd = pipefd_read[0];
                    pollfds[1].events = 0;
                    pollfds[1].events |= POLLIN | POLLHUP | POLLERR;

                    while(1)
                    {
                        p = poll(pollfds, 2, -1);
                        if (pollfds[0].revents & POLLIN)        // Input from the Terminal
                        {
                            byteCount_buf = read(STDIN_FILENO, buf, sizeof(buf));
                            if (buf[0] == CARRIAGE[0])
                            {
                                write(STDOUT_FILENO, NEWLINE, 2);
                                write(pipefd_write[1], LINEFEED, 1); 
                            }
                            else if (buf[0] == EOT)
                            {
                                /* Closing pipes to the shell */
                                close(pipefd_write[1]);
                            }
                            else if (buf[0] == INTERRUPT)
                            {
                                /* Closing pipes to the shell */
                                close(pipefd_write[1]);
                                kill(pid, SIGINT);
                            }
                            else
                            {
                                write(STDOUT_FILENO, buf, byteCount_buf);
                                write(pipefd_write[1], buf, byteCount_buf); 
                                
                            }
                        }
                        if (pollfds[1].revents & POLLIN)    // Output from the Shell
                        {
                            byteCount_message = read(pipefd_read[0], message, 1);
                            if (message[0] == LINEFEED[0])
                                write(STDIN_FILENO, NEWLINE, 2);
                            else
                                write(STDOUT_FILENO, message, byteCount_message);
                            
                        }
                        if(pollfds[1].revents & (POLLHUP | POLLERR))
                        {
                            close(pipefd_read[0]);
                            tcflush(STDIN_FILENO, TCIOFLUSH);
                            tcsetattr(STDOUT_FILENO, TCSANOW, original);
                            exit(EXIT_SUCCESS);
                        }
                    }
                    /* Wait for the Child process to terminate */
                    if (pid != 0)
                        waitpid(pid, NULL, 0);
                }
                break;
            default:
                exit(EXIT_SUCCESS);
        }
    }

    /* WITHOUT OPTION */
    while(1)
    {
        byteCount_buf = read(STDIN_FILENO, buf, sizeof(buf));
        if(buf[0] == CARRIAGE[0])  // Newline character
            write(STDOUT_FILENO, NEWLINE, 2);
        else if (buf[0] == EOT) // Detecting EOT/EOF or ctrl-D
        {
            
            tcflush(STDIN_FILENO, TCIOFLUSH);
            tcsetattr(STDOUT_FILENO, TCSANOW, original);
            kill(getpid(), SIGINT);
            break;
        }
        write(STDOUT_FILENO, buf, byteCount_buf);
    }

    /* CLEAN UP */
    tcflush(STDIN_FILENO, TCIOFLUSH);
    tcsetattr(STDOUT_FILENO, TCSANOW, original);

    return 0;

}