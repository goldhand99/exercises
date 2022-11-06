

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>





volatile int counter = 3;

void handler1(int sig) 
{

    sigset_t mask, prev_mask;
    
  unsigned char b[4];
 ;
    sigfillset(&mask);
     sigprocmask(SIG_BLOCK, &mask, &prev_mask);  /* Block sigs */
    write(STDOUT_FILENO,(void*)&counter,sizeof(counter));
     sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    _exit(0);
}



int main() 
{



    pid_t pid;
    sigset_t mask, prev_mask;

    printf("%d", counter);
    fflush(stdout);

    signal(SIGUSR1, handler1);
    if ((pid = fork()) == 0) {
        while(1) {};
    }
    kill(pid, SIGUSR1); 
    waitpid(-1, NULL, 0);

    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, &prev_mask);  /* Block sigs */
    printf("%d", ++counter);
    sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    exit(0);
}