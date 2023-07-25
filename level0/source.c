#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char *execv_args[2];
    uid_t uid;
    gid_t gid;

    if (atoi(argv[1]) == 423)
    {
        execv_args[0] = strdup("/bin/sh");
        execv_args[1] = NULL;

        uid = geteuid();
        gid = getegid();
        setreuid(uid, uid); // setresuid not working on my device
        setregid(gid, gid); // same

        execv("/bin/sh", execv_args);
    }
    else
        fwrite("No !\n", 5, 1, stderr);
}