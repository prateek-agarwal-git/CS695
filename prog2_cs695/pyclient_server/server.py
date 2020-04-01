import socket
import libvirt
import threading
H=["" for _ in range(6)]
H[1] = 'vm1'
H[2] = 'vm2'
H[3] = 'vm3'
H[4] = 'vm4'
H[5] = 'vm5'
a = 0
lock = threading.Lock()
CV = threading.Condition(lock)
flag = 0
# monitorThread(s) monitors the first s servers. It computes the CPU statistics 
# of first s servers. If the system is overloaded, then another vm is resumed.
# Resumption involves the steps:
# a) resuming from monitor s thread.
# b)informing the load balancer. The client is the load balancer.
# c) After receipt of OK message from client, change the flag variable
#  d) notify all.
# Questions: a) when will client start sending messages to the new server?
# client should maintain active socket connections
# and monitorThread(s+1) is called.
def monitorThread(s):
    # servers upto s should run
    global flag
    lock.acquire()
    while flag != s:
        CV.wait()
    # if (s > 0){
    S = 'vm'+str(s+1)
    print(S)
    global conn
    dom = conn.defineXML(D[S])
    dom.create()
    # }
    flag = s + 1
    CV.notify_all()
    lock.release()
conn = libvirt.open('qemu:///system')
domains = conn.listAllDomains(0)
D = {}
for domain in domains:
    D[domain.name()] = domain.XMLDesc()
# print(D.keys())
# print(D['vm1'][:100])
# dom = conn.defineXML(D['vm2'])
# dom.create()
thread = [threading.Thread(target=monitorThread,args=(i,))  for i in range(5)]
for i in range(5):
    thread[i].start()
for i in range(5):
    thread[i].join()











# _thread.start()
# domain = conn.lookupByName('thread3')
# x = domain.getCPUStats(True)
# y = domain.memoryStats()

# print(x)
# print(y)
# f = conn.listDomainsID()
# conn.listDomains() 
# print(f)
# print(domains)
# print(caps)



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
