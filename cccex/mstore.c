


long mult2(long, long); 


void multstore(long x, long y, long* dest){ 

long a = mult2(x,y); 
*dest = a; 
}