#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

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
    
    // printf("Size of original: %lu", sizeof(struct *termios));
    int result = tcgetattr(STDIN_FILENO, original);
    

    if (result == 0)
    {
        printf("Succeeded\n");
    }
    else
    {
        printf("Failed\n");
    }

    *modified = *original;

    printf("original c_iflag: %lu, c_oflag: %lu, c_lflag: %lu\n",
    original->c_iflag, original->c_oflag, original->c_lflag);

    printf("modified c_iflag: %lu, c_oflag: %lu, c_lflag: %lu\n",
    modified->c_iflag, modified->c_oflag, modified->c_lflag);
    
    modified->c_iflag = ISTRIP;
    modified->c_oflag = 0;  // Map NL to CR-NL on output
    modified->c_lflag = 0;    //set ICANON

    printf("original c_iflag: %lu, c_oflag: %lu, c_lflag: %lu\n",
    original->c_iflag, original->c_oflag, original->c_lflag);

    printf("modified c_iflag: %lu, c_oflag: %lu, c_lflag: %lu\n",
    modified->c_iflag, modified->c_oflag, modified->c_lflag);
    

    tcflush(STDIN_FILENO, TCIFLUSH);
    tcsetattr(STDIN_FILENO, TCSANOW, modified);

    while(1)
    {
        read(STDIN_FILENO, buf, 5);
        //printf("%d", buf[0]);
        if(buf[0] == '\r')
            write(STDOUT_FILENO, NEWLINE, 2);
        else if (buf[0] == EOT)
        {
            tcsetattr(STDOUT_FILENO, TCSANOW, original);
            exit(0);
        }

            

        write(STDOUT_FILENO, buf, 5);
    }
    tcflush(STDIN_FILENO, TCIOFLUSH);
    
    
    return 0;

}