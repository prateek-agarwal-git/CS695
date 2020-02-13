#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

int main(){
    char p[10] = "hello";
    char q[10];
    strcpy(q,p);
    printf("String is %s\n", q);
    printf("Address is %p\n",p);
    printf("&Address is %p",&p);

     return 0;
 }