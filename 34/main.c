// main.c
#include <stdio.h>
#include "lib.h"


int main(){
    int a,b,c;
    scanf("%d%d%d",&a,&b,&c);
    printf("max3 = %d\n",max3(a,b,c));
    return 0;
}
