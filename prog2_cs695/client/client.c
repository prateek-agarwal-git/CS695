#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
#include <assert.h>
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
long my_atoi(char * );
long  client_parser(char * );
int parameter;
thread_manager* manager; 
thread_manager* load_mode_manager;
// void input_mode(void * arg){
//      char c;
//      while ((c=getchar())!= 0){
//          if (c == '+'){
//              if (parameter <30) parameter++;
//          }
//          else if (c== '-'){
//              if (parameter > 5) parameter --;
//          }
//      }
//     return;
// }
// void load_balancer_interface(void * arg){
//     return;
// }
// void test_thread(void * arg){
//     while(1){ 
//         sleep(1);
//         printf("parameter: %d\n", parameter);
//     }
//     return;
// }
// void init_load_mode(void){
//     load_mode_manager = malloc( sizeof(thread_manager));
//     load_mode_manager->thread_count = 3;
// 	load_mode_manager->threads = (pthread_t*)malloc(load_mode_manager->thread_count * sizeof(pthread_t));
// 	pthread_create(&load_mode_manager->threads[0], NULL, (void*)input_mode, NULL);//mode: + and -  
// 	pthread_create(&load_mode_manager->threads[1], NULL, (void*)load_balancer_interface, NULL); //load: interact with load balancer
// 	pthread_create(&load_mode_manager->threads[2], NULL, (void*)test_thread, NULL); //test thread fr peinting value
//     for(int i=0; i<3; ++i) {
//             pthread_join(load_mode_manager->threads[i], NULL); 
//     }

// }
// int main(){
//     parameter = 10;
//     init_load_mode();


// }
int main(int argc, char * argv[]){
    client_args* C =(client_args *) malloc(sizeof(client_args));
    C->port_number = 8080;
    parameter = 25;
    strcpy(C->server_IP, "127.0.0.1");
    init_client(1,C);
    // start_client( C);
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
    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Why :");
        exit(1);
    }
    char * request_XML = (char*)malloc(MAXXMLSIZE);
    if (request_XML == NULL){
        perror("Error: ");
    }
    char * response_XML = (char*)malloc(MAXXMLSIZE);
    int length_data = MAXXMLSIZE;
    for (int t = 0 ; t < 10; t++){
        srand(t);
        memset(request_XML,0, length_data);
        int n = rand()%5 + parameter;
        printf("%d\n",n);
        client_converter(n, request_XML);
        length_data =strlen(request_XML);
        int datasent = send(sockfd, request_XML, length_data,0);
        if (datasent < 0){
            perror("Why: ");
        }
        while(datasent<length_data){
            int temp = send(sockfd, request_XML+datasent, length_data-datasent,0);
            datasent = datasent + temp;
        }
        memset(response_XML,0,MAXXMLSIZE);
        int bytes_count = recv(sockfd, response_XML,MAXXMLSIZE,0);
        char * temp1 = response_XML + bytes_count;
		while(strstr(response_XML, "</Response>")== NULL){
            bytes_count = recv(sockfd, temp1, MAXXMLSIZE, 0);
            temp1=  temp1 + bytes_count ;
		}
        long ans = client_parser(response_XML);
        printf("%ld\n", ans);
    }

}
void client_converter(int n, char * request_XML) {
    sprintf(request_XML,"<Request>%d</Request>\n",n);
    return;
}
long my_atoi(char * s){
    long ans = 0;
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
long client_parser(char * response_XML){
    int n = strlen("<Response>");
    for (int i = n; ; i++){
        if (response_XML[i] == '<'){
            response_XML[i] = '\0';
            break;
        }
    } 
    return my_atoi(response_XML+n);
}
// int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                        //   void *(*start_routine) (void *), void *arg);
// char * header = "<Request>";
//     char* end = "</Request>";
//     char string_integer[10]="";
//     my_itoa(string_integer, n);
//     request_XML[0] = '\0';
//     strcpy(request_XML,header);
//     strcat(request_XML,string_integer);
//     strcat(request_XML, end);
//     printf("%s\n", string_integer);
// //     void my_itoa(char * string_integer, int i){
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
