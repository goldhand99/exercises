#include <stdlib.h>
#include <stdio.h>

typedef unsigned char *byte_pointer; 



void show_bytes(byte_pointer start, size_t len){ 
    int i;
for(i = 0; i < len;i++){ 
printf("%.2x", start[i]);
printf("\n");
}
}

void show_float_bytes(float x){ 
show_bytes((byte_pointer) &x, sizeof(float));
}

void show_int_bytes(int x){ 
    printf("%d", x);
    printf("/n");
    printf("%p", &x); 
        printf("/n");
 

show_bytes((byte_pointer)&x, sizeof(int));
}


void show_void_values(void *x){ 
show_bytes((byte_pointer)&x, sizeof(void *));
}

int main(int argc, char** argv){
 float a = 0; 
show_float_bytes(a);

return 1;
}


