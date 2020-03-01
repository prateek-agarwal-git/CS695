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
void init_client(int thread_count, client_args * Cptr);

void start_client(void * arg );

struct client_start_args{
    int port_number;
    char server_IP[32];
};
typedef struct client_start_args client_args;