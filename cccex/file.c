#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sigint_handler(int sig)
{ 
    printf("Caught sigint! \n");
    printf("2");
    // exit(0);
}

int main(){ 
if(signal(SIGINT,sigint_handler) == SIG_ERR){ 
    printf("it did exit");
    exit(0);
}

printf("1");
pause();
printf("3");
return 0;
}