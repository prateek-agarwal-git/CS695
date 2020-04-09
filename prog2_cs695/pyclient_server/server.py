import socket
import libvirt
import threading
# server socket at the monitor station. 
# monitor will be waiting for client's request
# thread 0 will start vm1 and send the message to client that
# vm1 is ready and you may now send message to the vm1
# how long should it take to start a VM?
# whatever be the time, should client keep trying?
# yes  by seeing the return value.
# no if the above fails.
# load balancer should inform other clients that a new server is available 
# rather than opening sockets for it.
# client loadbalancer thread in c program:
# should send the request
# monitor is blocked for request on accept call.
#  client lb will wait on read call.
#  monitor will say vm1 is started
# load balance will increase a local variable
# client threads will send connect call to vm1
# clients will then write the data to the socket and expecta  reply.
# https://stackoverflow.com/questions/5616092/non-blocking-call-for-reading-descriptor
lock = threading.Lock()
CV = threading.Condition(lock)
flag = 1
server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind(('127.0.0.1',8888))
server_socket.listen(5)
conn1, addr = server_socket.accept()
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
overload = 1
def monitorThread(s):
    # servers upto s should run
    global flag
    lock.acquire()
    while flag != s:
        CV.wait()
    S = 'vm'+str(s)
    print(S)
    global conn
    dom = conn.defineXML(D[S])
    try:
        dom.create()
    except: 
        pass
    requeststr = '<Request>+</Request>'
    conn1.sendall(requeststr.encode())
    x = conn1.recv(1024)
    print(x.decode())
    flag = s + 1
    CV.notify_all()
    lock.release()
conn = libvirt.open('qemu:///system')
domains = conn.listAllDomains(0)
D = {}
for domain in domains:
    D[domain.name()] = domain.XMLDesc()
# print(type(D['vm1']))
# s = D['vm1'][:]
# print(s.find('vm1'))
# s= s[:28]+'vm7'+s[31:]
# dom1 = conn.defineXML(s)
# dom1.create()
thread = [threading.Thread(target=monitorThread,args=(i+1,))  for i in range(5)]
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

# i = 0
# while i<10:
	
# 	print(addr)
# 	data = conn.recv(1024)
# 	conn.send(str.encode('hi'))
# 	print(data)
# 	i+=1
# server_socket.close()
