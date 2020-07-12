#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

int main(int argc, char *argv[])
{

    struct termios *original = malloc(sizeof(*original));
    struct termios *modified = malloc(sizeof(*modified));
    char buf[5];
    int counter = 0;
    
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

    while(counter < 20)
    {
        read(STDIN_FILENO, buf, 5);
        write(STDOUT_FILENO, buf, 5);
        counter++;
    }

    tcsetattr(STDOUT_FILENO, TCSANOW, original);
    return 0;

}