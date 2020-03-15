#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <assert.h>
#include <pthread.h>
// cache libraries
 #include <arpa/inet.h>
#include <openssl/sha.h>
// server network libraries
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <net/if.h>

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sys/socket.h>
#define PORT_NUMBER 8080
#define SERVER_IP "127.0.0.1"
#define MAX_CLIENT_CONN 500
#define MAXXMLSIZE 256
void do_work(char *, int );
int main(int argc, char** argv) {
	struct sockaddr_in addr;
	struct sockaddr_in client_sockaddr_in;
	int addrlen, curr_clients = 0, on = 1;
	int sockfd, temp;
	// struct ifreq ifr;
	struct pollfd pollfds[MAX_CLIENT_CONN];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
		printf("Error in socket creation: %s\n", strerror(errno));
	}
    temp = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if(temp < 0) {
		perror("setsockopt() failed");
	    close(sockfd);
	    exit(-1);
	}
	temp = ioctl(sockfd, FIONBIO, (char *)&on);
    if(temp < 0) {
		perror("ioctl() failed");
		close(sockfd);
		exit(-1);
	}
    memset(&addr, 0, sizeof (addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUMBER);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		printf("Error in socket binding: %s\n", strerror(errno));
		close(sockfd);
		exit(-1);
	}
	if(listen(sockfd, MAX_CLIENT_CONN) == -1) {
		printf("Error in socket listen: %s\n", strerror(errno));
		close(sockfd);
		exit(-1);
	}

    printf("Server Running ...\n");
	addrlen = sizeof(client_sockaddr_in);
	memset(pollfds, 0 , sizeof(pollfds));
	size_t can_close[MAX_CLIENT_CONN];
	char * request_xml = (char *)malloc(MAXXMLSIZE);
    while(1){
        if(curr_clients < MAX_CLIENT_CONN) {
			memset(&client_sockaddr_in, 0, sizeof(client_sockaddr_in));
			while(1) {
				pollfds[curr_clients].fd = accept(sockfd, (struct sockaddr*)&client_sockaddr_in, &addrlen);
				if(pollfds[curr_clients].fd == -1) {
					if (errno != EWOULDBLOCK) {
						perror("error when accepting connection");
						close(sockfd);
						exit(1);
					}
					break;
				}
				else {
					pollfds[curr_clients].events = POLLIN;
					can_close[curr_clients] = 1;
					++curr_clients;
					printf("new client %d added: \n", curr_clients);
				}
			}
		}
		// printf("88\n\n");
        poll(pollfds, curr_clients, 1000);
		int total_clients = curr_clients, bytes_count;
        for(int i=0; i<total_clients; ++i) {
			if(pollfds[i].revents & POLLIN) {
				pollfds[i].revents = 0;
				memset(request_xml, 0, MAXXMLSIZE);
				bytes_count = recv(pollfds[i].fd, request_xml, MAXXMLSIZE, 0);
				if(bytes_count <= 0 && can_close[i]) {
					printf("client %d connection closed\n", i+1);
					close(pollfds[i].fd);
					pollfds[i].fd = -1;
					--curr_clients;
				} else {
					can_close[i] = 0;
					char * temp = request_xml + bytes_count;
					while(strstr(request_xml, "</Request>")== NULL){
						bytes_count = recv(pollfds[i].fd, temp, MAXXMLSIZE, 0);
						temp= temp + bytes_count;
					}
					do_work(request_xml, pollfds[i].fd);
					can_close[i] = 1;
				}
			}
        }
        if(curr_clients < total_clients) {
			int j = 0;
			for(int i=0; i<total_clients; ++i) {
				if(pollfds[i].fd != -1) {
					if(i != j) {
						pollfds[j] = pollfds[i];
						can_close[j] = can_close[i];
					}
					++j;
				}
 			}
		}
    }
	return 0;
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
void do_work(char * request_XML, int client_fd){
	int a = server_parser(request_XML);
	long ans= 0 ;
	printf("%d\n", a);
	if (a < 0) ans = 0;
	else{
		printf("Hi\n");
		for (long i = 0 ; i < (long) ((1L<<61)-1); i++){
			// printf("HI\n");
			if (1<<a  == i ){
				ans = (long) i;
				break;
			} 
		}
	}
	char response_XML[200];
    sprintf(response_XML, "<Response>%ld</Response>\n",ans);
	int length_data =strlen(response_XML);
    int datasent = send(client_fd, response_XML, length_data,0);
    // printf("datasent = %d  length_data = %d\n", length_data,datasent);
    while(datasent<length_data){
            // printf("hi write from thread pool\n");
            int temp = send(client_fd, response_XML+datasent, length_data-datasent,0);
            datasent = datasent + temp;
    }
	return;
}
