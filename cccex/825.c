#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>



jmp_buf buff; 

void handler(){ 
longjmp(buff,1);



}

char* tfgets(char * str, int n ,FILE * stream){ 

char *result; 

if   (!(setjmp(buff))){ 
if(signal(SIGALRM, handler) == SIG_ERR){ 
char * str= "File wrong";
write(STDOUT_FILENO,str,sizeof(str));
}
alarm(5);
return fgets(str,n,stream);
}else{ 
return NULL;
}
}


int main(int argc, char** argv, char**envp){ 
char buf[100];

char* input = tfgets(buf, 100, stdin);
if (input == NULL) { 
    printf("not typed");
}else { 
    printf("typed");
}
}