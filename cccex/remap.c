#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

int write_file(char *filename, char *content)
{
    int fd;
    char buf[20];
    umask(DEF_UMASK);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, DEF_MODE);
    write(fd, content, strlen(content));
    close(fd);
    return 0;
}

int main(int argc, char **argv)
{

    if (argc == 0)
    {
        return -1;
    }
    write_file("hello.txt", "Hello,world!");
    sleep(3);
    int fd = open("hello.txt", O_RDWR, 0);
    struct stat status;
    fstat(fd, &status);
    size_t size = status.st_size;
    char *bufp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    *bufp = 'J';
    write(1, bufp, status.st_size);
    munmap(bufp, size);
    return -1;
}