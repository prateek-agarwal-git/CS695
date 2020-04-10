import libvirt
from time import sleep
conn = libvirt.open('qemu:///system')
domains = conn.listAllDomains(0)
D = {}
for domain in domains:
    D[domain.name()] = domain.XMLDesc()
dom = conn.defineXML(D['vm1'])

dom.create()
sleep(30)
dom.shutdown()