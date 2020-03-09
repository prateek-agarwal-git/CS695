#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
int my_atoi(char * s){
    int ans = 0;
	char c;
    // printf("%s\n",s);
    // printf();
	while((*s)!= '\0') {
        if(((*s) >= '0') && ((*s) <= '9')) {
            ans = ans * 10 + ((*s)-'0');
        }
        else{
            printf("Invalid Integer\n");
            exit(1);
        }
        s++;
    }
	return ans;
}
int server_parser(char * request_XML){
    

    int n = strlen("<Request>");
    for (int i = n; ; i++){
        if (request_XML[i] == '<'){
            request_XML[i] = '\0';
            break;
        }
    } 
    return my_atoi(request_XML+n);
}
int main(){
    char req[100] = "<Request>12345</Request>";
    printf("%d\n", server_parser(req));
    return 0;
}