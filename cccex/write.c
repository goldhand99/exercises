 
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){ 

char * str= "Filewrong";
write(STDOUT_FILENO,str,strlen(str));
 }