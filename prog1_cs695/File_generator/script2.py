import sys, os
fp = open("f2.txt", "w")
c = 'a'
cint = ord(c)
for i in range(15):
    t = chr(cint)*19
    t += '\n'
    fp.write(t)
    cint+=1
fp.close()