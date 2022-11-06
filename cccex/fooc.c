#include "csapp.h"

#include <stdio.h>
#include "csapp.h"

ssize_t __rio_readn(int fd, void *buf, size_t size_b)
{

    size_t nleft = size_b;
    ssize_t nread;
    char *bufp = buf;
    while (nleft > 0)
    {
        nread = read(fd, bufp, size_b);
        if (nread == -1)
        {
            if (errno == EINTR)
            {
                nread = 0; //this is error
            }
            else
            {
                return -1; //error from reading the file
            }
        }
        else if (nread == 0) //means EOF
        {
            break;
        }
        else if (*bufp == '\n')
        {
            return -1;
        }
        nleft -= nread;
        bufp += nread;
    }

    return (size_b - nleft);
}

ssize_t __rio_writen(int fd, void *bufp, size_t n)
{

    size_t nleft = n;
    char *bufptr = bufp;
    size_t write_b;

    while (nleft > 0)
    {
        write_b = write(fd, bufptr, nleft);
        if (write_b == -1)
        {
            if (errno == EINTR)
            {
                write_b = 0;
            }
            else
            {
                return -1;
            }
        }
        nleft -= write_b;
        bufptr += write_b;
    }
    return n;
}

int main(int argc, char **argv)
{

    int n;
    rio_t rio;
    char buf[MAXLINE];
    ssize_t nr;

    while ((nr = __rio_readn(0, &buf, 1)) >= 0)
    {
        __rio_writen(1, &buf, 1);
    }
    __rio_writen(1, &buf, 3);
}
