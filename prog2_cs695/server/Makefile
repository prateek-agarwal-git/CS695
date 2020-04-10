all: server
	./server 192.168.122.21
server: server.c
	gcc ./server.c -lpthread -o server
clean: 
	rm server