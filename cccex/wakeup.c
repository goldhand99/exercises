

#include <stdio.h>


int main(int argc,char **argv, char **envp){ 

char *a = *argv;
char *b = *envp; 

while((*a)!= 'a')
{ 
    printf("%c",*(a));
    a++;
    break;
}
while ((*b)!= 'a')
{
  printf("%c",*(b));
    b++;
    break;
}



}