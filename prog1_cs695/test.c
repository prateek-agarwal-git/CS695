#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
#include <fcntl.h>
#include <errno.h>
int main(){
    // char p[10] = "hello";
    // char q[10];
    // strcpy(q,p);
    // printf("String is %s\n", q);
    // printf("Address is %p\n",p);
    // printf("&Address is %p",&p);
    int fp = open("abcd.txt", O_RDONLY);
    // printf("%d\n",fp);
    char A[100] = "\0";
    sprintf(A,"hello %s\n",strerror(errno));
    // if (fp <0){
    //     sprintf(A,"%s",perror("E:"));
    // }
    printf("%s",A);
     return 0;
 }