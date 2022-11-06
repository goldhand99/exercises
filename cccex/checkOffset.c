#include <stdio.h>
#include <assert.h>


struct a 
{
char c[16]; 
char* a[2]; 
// short cb[4]; 
// long* b; 
};

// int main(int argc, char ** argv){ 
// printf("a c %zu \n",__offsetof(struct a, c)); 
// printf("a a %zu \n",__offsetof(struct a, a)); 
// //printf("a cb %zu \n",__offsetof(struct a, cb)); 
// //printf("a b %zu \n",__offsetof(struct a, b));
// printf("%zu /n", sizeof(struct a));  
// }


// char* get_line(){ 

// char buf[4];
// char* result; 
// gets(buf);
// result = malloc(strlen(buf));
// strcpy(result, buf);
// return result; 

// }

// double funct3(int* ap, double b, long c, float* dp){ 

// double xmm2 = (double)*ap;
// float d = *dp; 


// if(b <= xmm2){ 
// b = (float)c + 2*d;
// return b; 
// }
// b = c * d; 
// return b; 
// }

// //3.58 

// long decode2(long x, long y, long z){ 
//     long result; 
// x *= (y-z); 
// result = (y<<63)>>63 ^ x;  
// return result; 
// }

// //3.59
// typedef __int128_t int_128t; 

// void store_prod(int_128t * dest, int64_t x, int64_t y){ 
// void* dest = &y;




// *dest = x * (int_128t)y; 
// return dest; 
// }

// //3.60 
// long loop(long x, long n){ 

// long result = 0;
// long mask; 

// for(mask = 1; mask != 0 ; mask <<= (short)n){ 
//     result |= (x & mask);
// }
// return result; 
// }

// //3.61 
// long cread_alt(long *xp){ 

//  return (!*xp ? 0 : xp); 

// }


// typedef enum {A,B,C,D,E} mode_t; 

// //3.62
// long switch3(long *p1, long* p2, mode_t action){ 

// long result = 0; 
// switch(action){ 

// case A: 
// result = *p2; 
// *p2 = *p1; 

// case B: 
// result = *p1 + *p2;
//  *p1 = result; 
// break;

// case C: 
// *p1 = 59; 
// result = *p2; 
// break; 

// case D: 
// *p1 = *p2; 
// result = 27;
// break;
// case E: 
// result = 27; 
// break;


// default: 
// result = 12; 
// break;

// }

// return result; 

// }


// //3.63
// long switch_prob(long x, long n){ 
// long result = x; 

// switch (n)
// {
//     case 60:
//     case 62: 
//     result = 8 * x; 
//     break;
//     case 63:
//     result = x >> 3;
//     case 64: 
//     result = x<<16 - x;
//     case 65: 
//     x *= x; 
// default:
// result = x + 75; 
//     break;
// }
// return result; 
// }

// //3.67

// typedef struct {
//     long a[2];
//     long *p;
// } strA;

// typedef struct 
// {
//  long u[2];
//     long q;
//     }strB ;



// strB process(strA s) {
//     strB r;
//     r.u[0] = s.a[1];
//     r.u[1] = s.a[0];
//     r.q = *s.p;
//     return r;
// }


//long eval(long x, long y, long z) {
//     strA s;
//     s.a[0] = x;
//     s.a[1] = y;
//     s.p = &z;
//     strB r = process(s);
//     long a = (r.u[0] + r.u[1] + r.q);
//     return a;
// }


#define buff_size 2

//3.71 
void good_echo(){ 
    while(1){
    char buf[buff_size]; 
    
        char* p = fgets(buf, 2, stdin);
        if(p == NULL) {
break;
        }
 printf("%s /n", p);
    }
      
    return;
}





typedef enum { neg, zero, pos, other} range_t; 

range_t find_range(float x){
__asm__(
    "vxorps %xmm1, %xmm1, %xmm1\n\t"
    "vucomiss %xmm1, %xmm0\n\t"
    "ja .N \n\t"
    "je .E \n\t"
    "jb .P \n\t"
    "jp .C \n\t"
    ".N: \n\t"
    "movl $0, %eax \n\t"
    "jmp .Done \n\t"
    ".E: \n\t"
    "movl $1, %eax \n\t"
    "jmp .Done \n\t"
    ".P: \n\t"
    "movl $2, %eax \n\t"
    "jmp .Done \n\t"
    ".C: \n\t"
    "movl $3, %eax \n\t"
    "jmp .Done \n\t"
    ".Done:\n\t"
    "ret \n\t"
);

}


range_t find_range(float x){ 
__asm__(
      "vxorps %xmm1, %xmm1, %xmm1\n\t"
      "movq $0, %rax \n\t"
      "movq $1, %edi \n\t"
      "movq $2, %esi \n\t"
      "movq $3, %edx \n\t"
      "vucomiss %xmm0, %xmm1 \n\t"
      "cmovaq %esi, %rax \n\t"
      "cmoveq %edi, %rax \n\t"
      "cmovpq %edx, %rax \n\t" 
);

}



int main(){ 



}



