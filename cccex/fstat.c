#include <stdio.h>
#include "csapp.h"

int main(int argc, char **argv)
{

    int fd;
    struct stat stat;

    if (argc == 0)
    {
        fd = 0;
    }
    else
    {
        fd = atoi(argv[1]);
    }
    fstat(fd, &stat);
}