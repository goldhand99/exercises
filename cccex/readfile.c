#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{

    char c[100];
    int a = 0;
    int fd = open("hello.txt", O_RDONLY, 0);

    while (read(fd, &c, 1) != 0)
    {
        a++;
        printf("%s \n", c);
    }
    write(1, &c, 100);
}
