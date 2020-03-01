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


typedef struct thread_manager{
    int thread_count;
	pthread_t* threads;
}thread_manager;

struct client_start_args{
    int port_number;
    char server_IP[32];
};
typedef struct client_start_args client_args;

void init_client(int thread_count, client_args * Cptr);

void start_client(void * arg );


thread_manager* manager; 
int main(int argc, char * argv[]){
    // if (argc <= 2) {
    //     printf("Unknown Error: Please follow the command line format i.e. ./object server_IP server_PORT\n");
    //     exit(1);
    // }
    // int server_port_number = read_integer(argv[2]);
    
    client_args* C =(client_args *) malloc(sizeof(client_args));
    C->port_number = 8080;
    strcpy(C->server_IP, "127.0.0.1");
    init_client(4, C);
    // start_client(server_port_number, argv[1]);  //port and IP  
    return 0;
}

void init_client(int thread_count, client_args * Cptr) {
	manager = (thread_manager*)malloc(sizeof(thread_manager));
	manager->thread_count = thread_count;
	manager->threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
	for(int i=0; i<thread_count; ++i) {
		pthread_create(&manager->threads[i], NULL, (void*)start_client, (void *)Cptr); 
		// pthread_detach(manager->threads[i]); 
	}
    for(int i=0; i<thread_count; ++i) {
		pthread_join(manager->threads[i], NULL); 
		// pthread_detach(manager->threads[i]); 
	}
     
}
void start_client(void * arg ){
    client_args C = *(client_args *) arg; // deep copy
    // struct sockaddr_in server_addr;
    // memset(&server_addr, 0, sizeof(server_addr));
    // int sockfd;
    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_port = htons(server_port_number);
    // server_addr.sin_addr.s_addr = inet_addr(server_ip);
    // 
    printf("%d, %s\n", C.port_number, C.server_IP);
   


}


// int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
//                           void *(*start_routine) (void *), void *arg);
