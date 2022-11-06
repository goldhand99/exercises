

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "csapp.h"

int main(int argc, char **argv)
{
    int n;

    rio_t rio;
    char buf[MAXLINE];
    int nr_in = 0;
    if (argc != 0)
    {
        nr_in = open(argv[1], O_CREAT | O_APPEND, 0);
    }
    Rio_readinitb(&rio, nr_in);
    while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
        Rio_writen(1, buf, n);
    }
}
