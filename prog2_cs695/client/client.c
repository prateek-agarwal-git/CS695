#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#define MAXXMLSIZE 256
typedef struct thread_manager{
    int thread_count;
	pthread_t* threads;
}thread_manager;
struct client_start_args{
    int port_number;
    char server_IP[32];
};
typedef struct client_start_args client_args;
void init_client(int, client_args * );
void start_client(void * );
void client_converter(int , char * );
int my_atoi(char * s);
int client_parser(char * response_XML);

thread_manager* manager; 
int main(int argc, char * argv[]){
    client_args* C =(client_args *) malloc(sizeof(client_args));
    C->port_number = 8080;
    strcpy(C->server_IP, "127.0.0.1");
    init_client(4, C);
    return 0;
}
void init_client(int thread_count, client_args * Cptr) {
	manager = (thread_manager*)malloc(sizeof(thread_manager));
	manager->thread_count = thread_count;
	manager->threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
	for(int i=0; i<thread_count; ++i) {
		pthread_create(&manager->threads[i], NULL, (void*)start_client, (void *)Cptr); 
	}
    for(int i=0; i<thread_count; ++i) {
		pthread_join(manager->threads[i], NULL); 
	}
}
void start_client(void * arg ){
    client_args C = *(client_args *) arg; // deep copy
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(C.port_number);
    server_addr.sin_addr.s_addr = inet_addr(C.server_IP);
    printf("%d, %s\n", C.port_number, C.server_IP);
   char * request_XML = (char*)malloc(MAXXMLSIZE);
    if (request_XML == NULL){
        perror("Error: ");
    }
    srand(0);
    int n = rand()%10000;
    client_converter(n, request_XML);

}
void client_converter(int n, char * request_XML) {
    sprintf(request_XML,"<Request>%d</Request>\n",n);
    return;
}
int my_atoi(char * s){
    int ans = 0;
	char c;
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
int client_parser(char * response_XML){
    int n = strlen("<Response>");
    for (int i = n; ; i++){
        if (request_XML[i] == '<'){
            request_XML[i] = '\0';
            break;
        }
    } 
    return my_atoi(request_XML+n);
}
// int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
//                           void *(*start_routine) (void *), void *arg);
// char * header = "<Request>";
    // char* end = "</Request>";
    // char string_integer[10]="";
    // my_itoa(string_integer, n);
    // request_XML[0] = '\0';
    // strcpy(request_XML,header);
    // strcat(request_XML,string_integer);
    // strcat(request_XML, end);
    // printf("%s\n", string_integer);
//     void my_itoa(char * string_integer, int i){
//     if (i == 0){
//         strcpy(string_integer, "0");
//         return;
//     }
//     int j = 0; 
//     while (i!= 0){
//         int temp = i%10;
//         i = i/10;
//         string_integer[j] = temp+'0';
//         j++;
//     }
//     printf("%s\n", string_integer);
//     string_integer[j] = 0;
//     j = j-1;
//     for (int i = 0; i <j;i++,j--){
//         char temp = string_integer[i];
//         string_integer[i] = string_integer[j];
//         string_integer[j] = temp;
//    }
//    return;
// }
// if (argc <= 2) {
    //     printf("Unknown Error: Please follow the command line format i.e. ./object server_IP server_PORT\n");
    //     exit(1);
    // }
    // int server_port_number = read_integer(argv[2]);
    // start_client(server_port_number, argv[1]);  //port and IP  
