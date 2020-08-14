#include <stdio.h>
#include <unistd.h>

int main()
{
    char buf[] = "Hello World\n";
    write(STDOUT_FILENO, buf, 13);
    return 0;
}