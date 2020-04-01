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
#define NUMBEROFSERVERS 5
#define SERVER_PORT_NUMBER 8080
#define MONITOR_PORT_NUMBER 8888 

typedef struct thread_manager{
    int thread_count;
	pthread_t* threads;
}thread_manager;
struct client_start_args{
    int port_number;
    char server_IP[32];
};
typedef struct client_start_args client_args;
void init_client(int, client_args ** );
void start_client(void * );
void client_converter(int , char * );
long my_atoi(char * );
long  client_parser(char * );
int parameter;
thread_manager* manager; 
thread_manager* load_mode_manager;
client_args** C;
void input_mode(void * arg){
     char c;
     while ((c=getchar())!= 0){
         if (c == '+'){
             if (parameter <50) parameter++;
         }
         else if (c== '-'){
             if (parameter > 5) parameter --;
         }
     }
    return;
}
void load_balancer_interface(void * arg){
    return;
}
void test_thread(void * arg){
    while(1){ 
        sleep(2);
        printf("parameter: %d\n", parameter);
    }
    return;
}
void init_load_mode(void){
    load_mode_manager = malloc( sizeof(thread_manager));
    load_mode_manager->thread_count = 3;
	load_mode_manager->threads = (pthread_t*)malloc(load_mode_manager->thread_count * sizeof(pthread_t));
	pthread_create(&load_mode_manager->threads[0], NULL, (void*)input_mode, NULL);//mode: + and -  
	pthread_create(&load_mode_manager->threads[1], NULL, (void*)load_balancer_interface, NULL); //load: interact with load balancer
	pthread_create(&load_mode_manager->threads[2], NULL, (void*)test_thread, NULL); //test thread fr peinting value
    // for(int i=0; i<3; ++i) {
    //         pthread_join(load_mode_manager->threads[i], NULL); 
    // }

}
// int main(){
//     parameter = 10;
//     init_load_mode();


// }
int main(int argc, char * argv[]){
    C =(client_args **) malloc((NUMBEROFSERVERS+1)*sizeof(client_args *));
    for (int i = 0; i <= NUMBEROFSERVERS; i++){
        C[i] = (client_args *)malloc(sizeof(client_args));
        if (C[i] == NULL){
            perror("malloc: ");
            exit(1);
        }
        C[i] ->port_number = 8080;
    }
    C[0]->port_number = 8888;//monitor port 
    parameter = 25;
    // if (argc<=2){
    //     printf("Enter IP as well\n");
    //     exit(1);
    // }
    strcpy(C[0]->server_IP, "127.0.0.1");
    strcpy(C[1]->server_IP, "192.168.122.21");
    strcpy(C[2]->server_IP, "192.168.122.22");
    strcpy(C[3]->server_IP, "192.168.122.23");
    strcpy(C[4]->server_IP, "192.168.122.24");
    strcpy(C[5]->server_IP, "192.168.122.25");
    // start_monitor(C[0]);
     init_load_mode();
    init_client(2,C);
   
    // start_client( C);
    return 0;
}
void init_client(int thread_count, client_args ** Cptr) {
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
    client_args** C = (client_args **) arg; // deep copy
    struct sockaddr_in server_addr[NUMBEROFSERVERS];
    memset(&server_addr, 0, sizeof(server_addr));
    int sockfd[NUMBEROFSERVERS+1];
    for(int i = 1; i<=NUMBEROFSERVERS;i++){
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        server_addr[i].sin_family = AF_INET;
        server_addr[i].sin_port = htons(C[i]->port_number);
        server_addr[i].sin_addr.s_addr = inet_addr(C[i]->server_IP);
    }
    
    // printf("%d, %s\n", C[1]->port_number, C[1]->server_IP);
    if(connect(sockfd[1], (struct sockaddr*)&server_addr[1], sizeof(server_addr[1])) == -1) {
        perror("Why :");
        exit(1);
    }
    if(connect(sockfd[2], (struct sockaddr*)&server_addr[2], sizeof(server_addr[2])) == -1) {
        perror("Why :");
        exit(1);
    }
    if(connect(sockfd[3], (struct sockaddr*)&server_addr[3], sizeof(server_addr[3])) == -1) {
        perror("Why :");
        exit(1);
    }
    if(connect(sockfd[4], (struct sockaddr*)&server_addr[4], sizeof(server_addr[4])) == -1) {
        perror("Why :");
        exit(1);
    }
    if(connect(sockfd[5], (struct sockaddr*)&server_addr[5], sizeof(server_addr[5])) == -1) {
        perror("Why :");
        exit(1);
    }
    char * request_XML = (char*)malloc(MAXXMLSIZE);
    if (request_XML == NULL){
        perror("Error: ");exit(1);
    }
    char * response_XML = (char*)malloc(MAXXMLSIZE);
    int length_data = MAXXMLSIZE;
    int turn = 0;
    srand(0);
    while(1){
        
        // printf("here\n");
        memset(request_XML,0, length_data);
        int n = rand()%5 + parameter;
        printf("%d\n",n);
        client_converter(n, request_XML);
        length_data =strlen(request_XML);
        int datasent = send(sockfd[turn + 1], request_XML, length_data,0);
        // assert(1==2);
        if (datasent < 0){
            
            perror("Why: ");
        }
        while(datasent<length_data){
            int temp = send(sockfd[turn+1], request_XML+datasent, length_data-datasent,0);
            datasent = datasent + temp;
        }
        memset(response_XML,0,MAXXMLSIZE);
        int bytes_count = recv(sockfd[turn+1], response_XML,MAXXMLSIZE,0);
        char * temp1 = response_XML + bytes_count;
		while(strstr(response_XML, "</Response>")== NULL){
            bytes_count = recv(sockfd[turn+1], temp1, MAXXMLSIZE, 0);
            temp1=  temp1 + bytes_count ;
		}
        long ans = client_parser(response_XML);
        printf("%ld\n", ans);
        turn++;
        turn = turn%2;
        // if (turn == NUMBEROFSERVERS) turn = 0;
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
