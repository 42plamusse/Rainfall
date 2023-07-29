#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char c[0x44];

void m(void)
{
    printf("%s - %d\n", c, (int)time(0));
}

int main(int argc, char **argv)
{
    char *a; // esp + 0x1c
    char *b; // esp + 0x18
    FILE *fd;

    a = malloc(0x8);
    *a = 1;
    *(&a + 0x4) = malloc(0x8);

    b = malloc(0x8);
    *b = 2;
    *(&b + 0x4) = malloc(0x8);

    strcpy(*(&a + 0x4), argv[1]);
    strcpy(*(&b + 0x4), argv[2]);

    fd = fopen("/home/user/level8/.pass", "r");
    fgets(c, 0x44, fd);
    puts("~~");
}