#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX, INT_MIN
#include <stdlib.h>  // for strtol


short test(short x, short y, short z){ 

short val = y + z - x; 
if (z>5){ 
    if(y>2){ 
    val = x/z; 
    } else { 
        val = x/y;
    }
}else if(z<3){
     val = z/y;
}

return val; 

}









int main(int argc, char** argv){ 
  int a,b; 
  scanf("%x", &a);
  scanf("%x", &b); 
    int sum = a+b;
    int sub = a-b; 

    printf("%x \n", sum);
    printf("%x", sub);
    return 1; 
    
}


