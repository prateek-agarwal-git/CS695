import socket as s

c = s.socket(s.AF_INET, s.SOCK_STREAM)
c.connect(('127.0.0.1', 8888))
msg = "hello"
i = 0
while i < 10:
    c.send(str.encode(str(i)))
    data = c.recv(1024)
    i+=1
    print(data)
# c.shutdown(1)
c.close()

