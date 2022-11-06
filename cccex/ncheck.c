#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    struct addrinfo *p, *listp, hints;
    char buf[8192];
    int rc, flags;

    if (argc != 2)
    {
        fprintf(stderr, "uncomplete domain %s", argv[0]);
    };

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0)
    {
        fprintf(stderr, "error trying to get addr info %s", argv[0]);
    };

    flags = NI_NUMERICSERV;
    for (p = listp; p; p = p->ai_next)
    {
        char *ptr = (char *)p->ai_addr;
        ptr = ptr + 4;
        struct in_addr *cst = (struct in_addr *)ptr;

        struct sockaddr_in *sckadr = (struct sockaddr_in *)p->ai_addr;
        inet_ntop(AF_INET, cst, buf, 8192);
        printf("This is the byte order %s\n", buf);

        inet_ntop(AF_INET, &sckadr->sin_addr, buf, 8192);
        printf("This is the byte order %s\n", buf);
    }
}