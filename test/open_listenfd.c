#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int Open_listenfd(char *port)
{

    struct addrinfo *hints, *listp, *p;
    int listenfd, optval = 1;
    memset(hints, 0, sizeof(struct addrinfo));
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
    getaddrinfo(NULL, port, hints, &listp);

    for (p = listp; p; p = p->ai_next)
    {

        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            continue;
        }
        //now try to bind
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
        {
            break;
        }
        close(listenfd);
    }
    if (!p)
    {
        return ~1;
    }

    if (listen(listenfd, 8) != 0)
    {
        return ~1;
    }
    return listenfd;
}