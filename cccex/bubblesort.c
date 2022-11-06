
#include <stdio.h>

long* bubble_a(long *data, long count){ 

long* last = data + count - 1; 

while(last > data){

long* i = data; 

while(i < last){

printf("%ld",*i);

if( *(i+1) < (*i) ){
long t = *(i+1);
*(i + 1) = *(i); 
*(i) = t; 
}
i++; 
}
last--;
}
return data;
}

int main(int argv, char** argc){ 
    long a[] = {1,4,6,10,2,3,5}; 
    long* b = bubble_a(a, sizeof(a)/sizeof(long));

    for(int i = 0; i < 7; i++){ 
printf("%ld",b[i]); 
    }
}