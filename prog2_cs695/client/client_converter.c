#include "client.h"
void my_itoa(char * string_integer, int i){
    if (i == 0){
        strcpy(string_integer, "0");
        return;
    }
    int j = 0; 
    while (i!= 0){
        int temp = i%10;
        i = i/10;
        string_integer[j] = temp+'0';
        j++;
    }
    printf("%s\n", string_integer);
    string_integer[j] = 0;
    j = j-1;
    for (int i = 0; i <j;i++,j--){
        char temp = string_integer[i];
        string_integer[i] = string_integer[j];
        string_integer[j] = temp;
   }
   return;
}
void client_converter(int n, char * request_XML) {
    char * header = "<Request>";
    char* end = "</Request>";
    char string_integer[10]="";
    my_itoa(string_integer, n);
    request_XML[0] = '\0';
    strcpy(request_XML,header);
    strcat(request_XML,string_integer);
    strcat(request_XML, end);
    printf("%s\n", string_integer);
}

// int main(){
//     int n = 1435;
//     char temp[10] = "";
//     my_itoa(temp, n);
//     printf("%s\n", temp);

//     return 0;
// }