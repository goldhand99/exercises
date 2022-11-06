
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int mysystem(char* command,char ** env) {
  pid_t pid;
  int status;

  if ((pid = fork()) == 0) {
    /* child process */
    char* argv[4] = { "", "-c", command, NULL };
    execve("/bin/sh", argv, env);
  }

  /* print child pid so we can kill it */
  printf("child pid: %d\n", pid);

  if (waitpid(pid, &status, 0) > 0) {
    /* exit normally */
    if (WIFEXITED(status))
      return WEXITSTATUS(status);

    /* exit by signal */
    if (WIFSIGNALED(status))
      return WTERMSIG(status);
  }
  return 0; 
}

int main(int argc, char* argv[],char **envp) {
  int code;

  code = mysystem("./exit",envp);
  printf("normally exit, code: %d\n", code); fflush(stdout);

  code = mysystem("./stay",envp);
  printf("exit caused by signal, code: %d\n", code); fflush(stdout);
  return 0;
}












void try(){ 

    if(fork() != 0){ 
        fork();
        fork();
        printf("abcd \n");
        exit(0);
    }
return;

}


