#include <stdio.h>
#include <string.h>
char monitor_parser(char * request_XML){
    char * temp = request_XML+ strlen("<Request>");
    return temp[0];
}

int main(){
    char A[100] = "<Request>-</Request>";
    char c = monitor_parser(A);
    printf("%c\n", c);

}