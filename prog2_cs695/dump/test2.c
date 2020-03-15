#include <stdio.h>
#include <stdlib.h>
#include<time.h>
int main(){
    srand(time(0));
    int n = rand()%88888;
    printf("%d\n", n);
    char  * request_XML =(char *) malloc(200*sizeof(char));
    sprintf(request_XML,"<Request>%d</Request>\n", n);

    printf("%s", request_XML);
    return 0;
}