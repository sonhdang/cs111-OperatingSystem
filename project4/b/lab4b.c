#include <stdio.h>
#include <stdlib.h>
#include <mraa.h>

int main(int argc, char* args[])
{
    printf("MRAA version: %s\n", mraa_get_version());
    return 0;
}