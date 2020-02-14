import sys, os
fp = open("f4.txt", "w")
c = 'a'
cint = ord(c)
for i in range(1):
    t = "my_name_is_prateek_?"
    t += '\n'
    fp.write(t)
    cint+=1
fp.close()