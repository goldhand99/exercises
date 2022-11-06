#include <stdio.h>

//3.21
short test(short x, short y){ 
short val = 12+y; 
if (x<0){ 
    if(x<y){ 
val = x*y; 

    }else{ 
val = x^y; 

    }
}else if(y <= 10){ 
val = x/y;
}
return val; 

}
//3.24 
short loop_while(short a, short b){ 
    short result =  0; 
    while(a > b){

result += a+b; 
a -= 1; 
    }
return result; 
}
short test_two(unsigned short x){ 

short val = 0; 
short i; 
for(i = 16 ; x!=0;i--){
val = (val<<1) | (x & 1);
x>>=1;
}
return val; 

}
//3.31

void switcher(long a, long b, long c, long *dest){ 


long val;
switch(a){ 
    case  5: 
    c = a ^ 15;

    case 0: 
      b = 112 + c; 
      break;

    case 2:
    case 7: 
    b = (a+c)<<2;
    break;
    case 4: 
    *dest = b;

default: 
a = b; 
}
dest = &val;
}

long rfun(unsigned long x){  

if((x & 0x1)^0x1) return 0; 

unsigned long nx = x >> 2; 
long rv = rfun(nx); 
return x + rfun(nx);

}


#define N 16 
typedef int fix_matrix[N][N]; 


void fix_set_diag_prep(fix_matrix A, int val){ 

int *last = &A[0][0]; 
long i = 0; 
long iend = N*(N+1); 
do{ 

last[i]= val; 
i += (4*(N+1)); 

}while(i != iend);

}



long uu2double(unsigned word0, unsigned word1){ 
    union{ 
long d; 
unsigned u[2]; 
    } temp;
    temp.u[0] = word0;
    temp.u[1] = word1; 

    return temp.d; 
}



typedef char* chr_point; 

void printBytes(chr_point a){ 
int i; 
for(i = 0; i<4; i++){ 
    printf("%.2x", a[i]); 
}
}






int main(int argc, char** argv){ 

int a = 0x10000000; 
printBytes((chr_point)&a); 


}


struct ACE { 
    short val; 
    struct ACE* p; 
}; 

short short_test(struct ACE* ptr){ 
short r = 1; 
while(ptr != NULL){ 
r = ptr->val * r; 
ptr = ptr->p; 
}
return r; 
}





