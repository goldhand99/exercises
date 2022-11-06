#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>


int saturating_add(int x, int y){ 
 //check for positive overflow 
 int sum = x + y; 
 if (x >=0 && y>=0 && sum<0) return __INT_MAX__;
 if(x<0 && y<0 && sum>=0) return (__INT_MAX__ -1);
return sum; 
}


int t_sub_ok(int x, int y){ 
return saturating_add(x, -y); 
}


int signed_high_prod(int x, int y){ 
int64_t produs = (int64_t)x * y;
return produs >> 32; 
}



unsigned unsigned_high_prod(unsigned x, unsigned y){ 
int sign_x = x>>31; 
int sign_y = y>>31; 
int prod = signed_high_prod(x,y);
return  prod + sign_x*y + sign_y*x;
}


// void *malloc(size_t size);
// void *memset(void *s, int c, size_t n);


void* calloc(size_t nme , size_t size){ 

if(nme == 0 || size == 0){ 
return NULL;
}

size_t buffSize = nme*size; 
//verific daca a depasit overflow 
if(buffSize/nme == size){ 
void *mem = malloc(buffSize);
if(mem == NULL){ 
    return NULL; 
}
return mem;
}
    return NULL;

}


int A(int x){
     return (x<<4) + x;
}

int B(int x){
    return x - (x<<3);  

}
int C(int x){
    return (x<<6) - (x<<2);
}

int D(int x){
    return (x<<4) - (x<<7);
}

int divide_power2(int x,int k){ 

// int neg_flag = x & INT32_MIN;
// neg_flag && x = (x+(1<<k)-1);
 int neg_flag = x&INT32_MIN;
 neg_flag && (x=(x + (1<<k) -1)); 
return x>>k; 
}


int mul3div4(int x){ 
x = (x<<1) + x; 
int neg_flag = x & INT32_MIN; 
neg_flag && (x = x+3);
return (x>>2);
}

void cititMatrice(int linii, int coloane, int x){ 
int i,a; 
int matrix [linii][coloane];
int suma = 0; 


for(i = 0; i<linii; i++){ 

for(a = 0;a< coloane; a++){ 
if(i == x){ 
    suma += matrix[i][a];
}
printf("%d", matrix[i][a]);
}
}
printf("%d", suma);
}


//Write code for a function threefourths which, for integer argument x, 
//computes the value of (3x/4), rounded toward zero. It should not overflow.
//first divide and then after multiply 

//3x = 2^1 + 1; 

//3 * x/4

int three_fourths(int x){ 
int map_flag = x & INT32_MIN; 
map_flag && (x = x+3);
x = (x>>2);
//now multiply by 3 
return  ((x<<1) + x); 
}


unsigned f2u(float x){ 
    return *(unsigned *)&x; 
}

int float_le(float x, float y){ 
unsigned ux = f2u(x);
unsigned uy = f2u(y); 

unsigned sx = ux >> 31; 
unsigned sy = uy >> 31; 

return ((ux<<1) ==(uy <<1)) ||((ux<uy) && (sx>sy))||((ux > uy) && (sx<sy));

}

float fpwr2(int x){ 

 unsigned exp, frac; 
 unsigned u; 

 if(x < -149 ){
     //normalized
     exp = 0; 
     frac = 0; 
 } else if(x < -126){
     //denormalized
     exp = 0; 
     frac = 1 << (unsigned)(x + 149);
     
 }else if(x<127){ 
exp = x + 127; 
frac = 0; 
 }else { 
     exp = 0xFF; 
     frac = 0; 
 }

u = exp<<23 | frac; 
return (float) u;
}


typedef unsigned float_bits; 
float_bits float_negate(float_bits f){ 
//check to see if it is nan 
int check = 0x7fffffff ^ f;  
if(((check<<1)>>1) == 0){
    return f;
}
float_bits sign_bit = (~f >>31)<<31; 
float_bits e = ((f<<1)>>25)<<23;
float_bits frac = (f<<9)>>9; 
return(sign_bit | e | frac);

}

float_bits float_absval(float_bits f){ 
float_bits ret = (f << 1)>>1; 
if (!(ret ^ ~0x7fffffff)){ 
    return f; 
}
return ret; 
}


float_bits float_twice(float_bits f){ 
float_bits sign_bit = (f>>31)<<31; 
float_bits exp = (f<<1)>>25;
float_bits frac = (f<<9)>>9; 
if ((exp ^ ~0xFF) && (frac ^ ~0x7ff)){ 
return f; 
}
if(exp == 0xff - 1){ 
    frac = 0; 
}else if(exp == 0){ 
    frac = frac<<1;
}else{ 
    exp = exp + 1; 
} 

return sign_bit || (exp << 23) || (frac);
}

float_bits float_half(float_bits f){ 
float_bits sign_bit = (f>>31)<<31; 
float_bits exp = f>>23 & 0xff;
float_bits frac =  f & 0x7FFFFF;
float_bits rest = f * 0xffffff;
if ((exp ^ ~0xFF) && (frac ^ ~0x7ff)){ 
return f; 
}

int addition = (frac & 0x3) == 0x3;

if(exp == 1){
      //normalized to denormalized
    exp = 0; 
    rest = rest >> 1; 
    rest += 1; 
    frac = rest & 0x7fffff; 
}else if (exp == 0){ 
frac = frac >> 1; 
frac += 1; 
}else { 
    exp -= 1; 
}





return (sign_bit | exp | frac); 

}

int float_f2i(float_bits f) {
unsigned exp = f>>23 & 0xff; 
unsigned sign_bit = f>>31; 
unsigned frac = f & 0x7fffff; 
int bias = (1<<6) - 1;
int result ;  

if(exp == 0xff) return 0x80000000;
if(exp == 0){ 
     result =((1-bias)*frac); 
}else{ 
if(exp>bias){ 
    result =  (1<<(exp - bias)) * (frac | 0x800000);
}else if(exp<bias){ 
result = (1>>(bias-exp)) * (frac | 0x800000);
}
return sign_bit ? -result : result; 

};

return 0x0;
}


//convert integer to float 
float_bits float_i2f(int i){ 
   unsigned x  = (unsigned) i; 
   unsigned sign_bit = (x>>31)<<31; 
   unsigned bias = 127;
   float_bits result; 
   unsigned shifts = 1; 
   while((x>>shifts) != 0x1 && (shifts <32)){ 
   shifts++; 
   };

   if(i == 0){ 
       return 0x00000000;
   }else if(i == INT32_MIN){ 
sign_bit = 0; 
unsigned exp = 31 + bias; 
unsigned frac = 0; 
return (0<<31)|(exp<<23)|frac;
   }

   if(shifts <= 23){
       unsigned moves = 31 - shifts; 
       unsigned exp = (shifts + bias);
       unsigned frac = ((x<<(moves))>>(moves)) & 0x7ffff; 
       return (sign_bit)|(exp<<23)|frac;
    }
 else if(shifts >23){ 
unsigned offset = shifts - 23; 
unsigned exp = (shifts + bias);
unsigned move_bits_to_get_frac = (32 - shifts + 1); 
unsigned fraction = (x<< move_bits_to_get_frac)>>(move_bits_to_get_frac);
unsigned frac = fraction >> shifts; 
unsigned middle_number = ((frac<<1) + 1);
unsigned round_up  = (frac + 1);
if(middle_number < fraction){ 

}else if(middle_number > fraction){ 
    frac += 1; 
}else { 
if( frac & 0x0 == 0x0){ 
      return (sign_bit)|(exp<<23)|frac;
}else{ 
       return (sign_bit)|(exp<<23)|round_up;
}
return 0;
}

 }
return 0b0; 
}






float_bits float_i2f(int i){ 










}









int main(int argc, char** argv){ 
    printf("works");
    }

