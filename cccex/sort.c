void bubblesort(long*begin, long*end){


for(long t, *p,*q;begin != end; end--)
for(q = begin, p = q++;p != end;p = q++)
if(*p > *q) t = *p, *p = *q , *q = t  ; 
}

typedef long size_t;


void * basic_memset(void * s, unsigned long c, size_t n){ 

size_t cnt = 0; 
unsigned char* schar = s; 
while(cnt < n){ 
    if((size_t)schar % 8 == 0){ 
        break; 
    }
*schar++ = (unsigned char) c; 
cnt++;  
}
return s; 
}


double poly_6a(double a[], double x, long degree){ 

long i; 
double result = *(a + 0);
double xpwr = x; 
double xpwr1 = x * x; 
double xpwr2 = x * x * x; 
unsigned int a = degree - 6
for(i = 0; i < a; i+=5){



}









}


