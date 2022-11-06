#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int open_clientfd(char *hostname, char *port)
{

    int clientfd;
    struct addrinfo *hints, *listp, *p;
    int rc, flags;
    memset(hints, 0, sizeof(struct addrinfo));
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;

    if ((rc = getaddrinfo(hostname, NULL, hints, &listp)) < 0)
    {
        fprintf(stdout, "FUCKING ERROR", NULL);
    }

    flags = NI_NUMERICHOST;
    for (p = listp; p; p = p->ai_next)
    {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            continue;
        }

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
        {
            fprintf(stdout, "Connected", NULL);
            break;
        }
        close(clientfd);
    };
    freeaddrinfo(listp);
    if (!p)
        return -1;
    else
        return clientfd;
}
