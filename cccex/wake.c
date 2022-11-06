

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void handler(int sig){ 
    return;
}

void snooze( int secs){ 
 int rc = sleep(secs);
printf("Slept for %d out of %d seconds", rc,secs);
}



int main(int argc, char **argv){
if (argc != 2){ 

fprintf(stderr,"usage : %c <secs>\n",*(*argv));
    exit(0);
}

if(signal(SIGINT,handler)==SIG_ERR){ 
printf("error in registering the handler");
exit(0);
}

printf("%s",argv[1]);
snooze(atoi(argv[1]));
exit(0);

}