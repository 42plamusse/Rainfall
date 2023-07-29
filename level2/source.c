#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void p(void)
{
    char buffer[0x40];
    void *return_addr;

    fflush(stdout);
    gets(buffer);
    return_addr = __builtin_return_address(0);

    if (((unsigned int)return_addr & 0xb0000000) == 0xb0000000)
    {
        printf("%p\n", return_addr);
        exit(1);
    }
    else
    {
        puts(buffer);
        strdup(buffer);
    }
}

int main(void)
{
    p();
    return (0);
}