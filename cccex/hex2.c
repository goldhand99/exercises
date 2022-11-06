
#include "csapp.h"

int main(int argc, char **argv)
{

    struct in_addr inaddr;
    uint32_t addr;
    char buf[MAXBUF];
    addr = strtol(argv[1], NULL, 10);

    char str[100];
    sprintf(str, "%d", 1024);
    printf("%s\n", str);
    printf("%ld\n", strtol(str, NULL, 2));

    printf("this is the addr %u\n", addr);

    inaddr.s_addr = htonl(addr); //convert it from host byte order to newtork byte order
    printf("%u\n", ntohl(inaddr.s_addr));
    inet_ntop(AF_INET, &inaddr, buf, MAXBUF);
    printf("%s\n", buf);
}