
#include <stdio.h>
#include <unistd.h>

#define MAXBUF 128


int main(){ 
char buf[MAXBUF];
while (1)
{
    while(read(STDIN_FILENO,buf,sizeof(buf))==0){ 
for(int i = 0; i < 128; i++){ 
    printf("%c\n",*(buf + i));
}
    }



  
}





}