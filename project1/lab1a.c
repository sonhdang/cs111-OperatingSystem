#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    // Allocating memory for temios structs
    struct termios *original = malloc(sizeof(*original));
    struct termios *modified = malloc(sizeof(*modified));
    // Setting buffer size to 5 to account for buffering
    char buf[5];
    // Constant for Carriage Return and Line Feed <cr><lf>
    const char NEWLINE[2] = {'\r','\n'};
    const char EOT = 0x04;
    int c;
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
                printf("hello %s\n", optarg);
                break;
            default:
                abort ();
        }
    }

    tcgetattr(STDIN_FILENO, original);

    *modified = *original;
    modified->c_iflag = ISTRIP;
    modified->c_oflag = 0;
    modified->c_lflag = 0;
    

    tcflush(STDIN_FILENO, TCIFLUSH);
    tcsetattr(STDIN_FILENO, TCSANOW, modified);

    while(1)
    {
        read(STDIN_FILENO, buf, 5);
        if(buf[0] == '\r')  // Newline character
            write(STDOUT_FILENO, NEWLINE, 2);
        else if (buf[0] == EOT) // Detecting EOT/EOF or ctrl-D
        {
            tcsetattr(STDOUT_FILENO, TCSANOW, original);
            exit(0);
        }
        write(STDOUT_FILENO, buf, 5);
    }

    tcflush(STDIN_FILENO, TCIOFLUSH);
    
    
    return 0;

}