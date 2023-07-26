#include <stdio.h>
#include <stdlib.h>

void run(void)
{
    fwrite("Good... Wait what?\n", 0x13, 1, stdout);

    system("/bin/sh");
}

int main(void)
{
    char buffer[0x40];

    gets(buffer);
}