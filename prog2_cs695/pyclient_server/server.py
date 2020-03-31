import socket
import libvirt
import threading

def clientThread():
    pass

conn = libvirt.open('qemu:///system')
caps = conn.getCapabilities()
print(caps)



# print('balle')
# x = 3/2
# print(x)
# server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
# server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# server_socket.bind(('127.0.0.1',8888))
# server_socket.listen(5)
# conn, addr = server_socket.accept()
# i = 0
# while i<10:
	
# 	print(addr)
# 	data = conn.recv(1024)
# 	conn.send(str.encode('hi'))
# 	print(data)
# 	i+=1
# server_socket.close()
