all: server
	./server 192.168.122.23
server: server.c
	gcc ./server.c -lpthread -o server
clean: 
	rm server