#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  char a[10], b[10];
  char *ptr, *ptrr;
  ptr = a;
  ptr = b;

  for (int i = 0; i < 9; i++)
  {
    *(ptr++) = 'c';
  }
  *(ptr) = '\0';
  printf("%s", a);
  for (int i = 0; i < 9; i++)
  {
    *(ptr++) = 'b';
  }
  *(ptr) = '\0';
  printf("%s", a);

  // for (int i = 0; i < 3; i++)
  // {
  //   fork();
  // }
  // printf("exmaple");
  // _exit(0);
}