#!/bin/bash
uname=`uname -a`
if [[ $uname == *"armv7l"* ]]
then
    gcc -Wall -Wextra -lmraa -lm -o lab4b lab4b.c
else
    gcc -Wall -Wextra -lm -o lab4b -DDUMMY lab4b.c 
fi