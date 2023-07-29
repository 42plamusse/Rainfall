#include <stdio.h>
#include <stdlib.h>

void p(void)
{
    system("/bin/sh");
    exit(1);
}

void n(void)
{
    char buffer[0x200];

    fgets(buffer, 0x200, stdin);
    printf(buffer);
    exit(1);
}

int main(void)
{
    n();

    return (0);
}