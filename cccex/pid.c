#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(){ 

    int status; 


if(fork()==0){ 
printf("9\n");fflush(stdout);
}else { 
printf("0\n");fflush(stdout);
int a = waitpid(-1,&status,0);
if(WIFEXITED(status) == 0){ 
    printf("has bene terminated by exit signlat the child  %d" , a); 
}


}
printf("3\n");fflush(stdout);
printf("6\n");exit(0);

}