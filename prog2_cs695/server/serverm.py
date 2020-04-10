import socket
server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.setblocking(0)
server_socket.bind(('192.168.122.21',8080))
server_socket.listen(5)

while True:
    