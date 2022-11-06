#include <stdio.h>

int is_arithmethic(){ 
int i = -1; 
int a = i >> 24; 
printf("%d", a);
if(a>0) return 1; 
return 0; 

}

unsigned srl(unsigned x, int k){ 
    unsigned xsra = (int) x >> k;
    int a = 0x00 << (32-k); 
    return xsra & a;
}

int sra(int x, int k){ 

int xsrl = (unsigned) x >> k; 
unsigned a = 0xff << (32-k); 
return (int)(xsrl | a); 

}


int add_ones(unsigned x){ 

return ((x)|(x>>8)|(x>>16)|(x>>24)) & 0x55;
}

int leftmost_one(unsigned x){ 
 x |= x >>1; 
x |= x >>2; 
 x |= x >>4; 
x |= x >>8;
x |= x >>16; 
x |= x>>24;

return x ^= (x >>1);

}

int lower_one_mask(int n){ 
int x = 1 << 31; 
if (n < 32){ 
x = 1 >> (32 - n); 
x = ~x; 
}
else{ 
x = -1;
}
return x; 
}

int do_rotating_left_shift(unsigned x, int n){ 
    if (n==0) return x;
return ((x<<n)|(x>>(32-n)));
}

int fits_bits(int x, int n){ 
int c,r; 
c = 33 + ~n; 
r = !(((x << c)>>c)^x) ; 
return r; 
}


typedef unsigned packed_t; 

int xbyte(packed_t word, int bytenum){ 

int size = sizeof(unsigned); 
printf("%d", size);
int leftshift = (size - 1 - bytenum)<<3; 
int rightshifts = 24; 

return (int)(word<<leftshift)>>rightshifts;

}


void copy_int(int val, void *buf, int maxbytes){ 
if(maxbytes - (int)sizeof(int)>= 0 ) return; 
}






int main(int argc, char** argv){ 
int a = xbyte(0x88888888,1);

printf("%x", a);
}