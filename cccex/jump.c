#include <stdio.h>
#include <setjmp.h>


jmp_buf buf; 
int error1 = 0; 
int error2 = 1; 

void foo(void), bar(void); 

int main(){
     
     switch (setjmp(buf))
     {
     case 0: 
     foo();
         break;
     
     case 1:
         printf("naso");
         break;
         case 2: 
         printf("lamol");
         break;
         default : 
         break;
     }
}

     void foo(void){ 

if(error1){ 
    longjmp(buf, 1);
}
bar();
     }

     void bar(void){ 
if(error2){ 
    longjmp(buf,2);
}

     }




