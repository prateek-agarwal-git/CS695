#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

int main(){
    char p[10] = "hello";
    printf("String is %s\n", p);
    printf("Address is %p\n",p);
    printf("&Address is %p",&p);

     return 0;
 }