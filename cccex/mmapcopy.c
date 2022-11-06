#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    if (argc == 0)
    {
        return 0;
    }
    char *str = argv[1];
    int fd;
    struct stat stat;

    fd = open(str, O_RDWR);

    

    if (fd == -1)
    {
        printf("File not found");
        return -1;
    }
    fstat(fd, &stat);

    void *bufp = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    write(1, bufp, stat.st_size);
}