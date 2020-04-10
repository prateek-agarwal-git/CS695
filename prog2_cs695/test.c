#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main(){
	struct timespec t;
	t.tv_sec = 0;
	t.tv_nsec = 500000000;
	while(1){
	nanosleep(&t, NULL);
	printf("hi\n");
	}

}
