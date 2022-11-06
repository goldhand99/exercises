#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>




#define N 2


    int main() {
      
           int status, i;
           pid_t pid[N], ret_pid;

           /* Parent creates N children */
           for(i = 0; i < N; i++) {
                   if((pid[i] = fork()) == 0) { /* child */
                           strcpy("", "World");
                           //orwite to a null pointer  /* Writing on read-only location. It creates SIGSEGV */
                           exit(100 + i);
                   }
           }

           /* Parent reaps N children in order */
           i = 0;
           while ((ret_pid = waitpid(pid[i++], &status, 0)) > 0)
           {
                   if(WIFEXITED(status))
                           printf("child %d terminated normally with exit status=%d\n", ret_pid, WEXITSTATUS(status));
                   else if(WIFSIGNALED(status)) { /* Exited with a signal */
                           printf("child %d terminated by signal %d: \n", ret_pid, SIGSEGV);
                           psignal(WTERMSIG(status), NULL);
                   }
                   else
                           printf("child %d terminated abnormally \n", ret_pid);
           }

           /* The only normal termination is if there are no more children */
          

           exit(0);
   }
 