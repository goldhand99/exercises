
#include <stdio.h>



void convert(unsigned char*bytes,unsigned long n){ 

bytes[0] = (n >> 24) & 0xFF;
bytes[1] = (n >> 16) & 0xFF;
bytes[2] = (n >> 8) & 0xFF;
bytes[3] = n & 0xFF;
}



int main(){ 
    unsigned char ba[4];
    unsigned long b = (2^32-1); 
    convert(ba,b);
    for(int i = 0; i<4; i++){ 
        printf("%.2x \n",ba[i]);
    }

}