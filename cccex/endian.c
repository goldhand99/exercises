#include <stdio.h>

typedef unsigned char *byte_pointer;




int show_bytes(int x){ 
byte_pointer a = (byte_pointer)&x; 
int i;
printf("%s",a);
if(a[0] == 0x01) return 1; 
return 0;  
}

int isEndian(){ 
int i = 16; 
return show_bytes(i);
}

int main(int argc, char**argv){
int a = isEndian();
printf("%d",a);


}