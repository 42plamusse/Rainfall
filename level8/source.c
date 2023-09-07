#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **auth;
char **service;

int main(void)
{
    char buffer[0x80];

    while (1)
    {
        printf("%p, %p \n", auth, service);
        if (!fgets(buffer, 128, stdin))
            break;

        if (!memcmp(buffer, "auth ", 0x5))
        {
            *auth = (char *)malloc(0x4);
            **auth = '\0';
            if (strlen(buffer) <= 0x1e)
            {
                strcpy(*auth, buffer + 0x5);
            }
        }
        if (!memcmp(buffer, "reset", 0x5))
        {
            free(*auth);
        }
        if (!memcmp(buffer, "service", 0x7))
        {
            strcpy(*service, buffer + 0x7);
        }
        if (!memcmp(buffer, "login", 0x5))
        {
            if (*auth[0x20] != '\0')
                system("/bin/sh");
            else
                fwrite("Password:\n", 1, 0xA, stdout);
        }
    }
}