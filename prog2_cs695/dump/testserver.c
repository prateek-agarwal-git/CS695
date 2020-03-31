#include <string.h>
#include <stdio.h>

int main(){
    char temp[200];
    int a = 2;
    sprintf(temp, "<Response>%d</Response>\n",a);
    printf("%s",temp);


    return 0;
}