import libvirt
import sys
conn = libvirt.open("qemu:///system")
# print(conn)
if conn == None:
    print("connection failed")
# cap = conn.getCapabilities()
# print("capabilities:\n"+cap)
host = conn.getHostname()
print('hostname: ' + host)
vcpus = conn.getMaxVcpus(None)
print('Vcpus: ' + str(vcpus))
nodeInfo = conn.getInfo()
print(nodeInfo)
print("[model, memory size, number of cpus, mhz, number of numa nodes, CPU sockets, CPU cores per socket,CPU threads per core]")
numNodes = nodeInfo[4]
memcell = conn.getCellsFreeMemory(0, numNodes)
cell = 0
for m in memcell:
    print('Node '+ str(cell) + ': ' + str(m)+ ' free bytes in memory' )
    cell+=1
t = conn.getType()
print("type: " +t)
ver = conn.getVersion()
print("version: " +str(ver))
Libver = conn.getLibVersion()
print("version: " +str(Libver))
uri = conn.getURI()
print("uri: " +str(uri))
isEncrypted = conn.isEncrypted()
print("Is the connection encrypted?\n"+ str(isEncrypted))
isSecure = conn.isSecure()
print("Is the connection secure?\n"+ str(isSecure))
isAlive = conn.isAlive()
print("Is the connection alive?\n"+ str(isAlive))
xml = '<cpu mode="custom" match="exact">' +'<model fallback="forbid">kvm64</model>' +'</cpu>'
retc = conn.compareCPU(xml)
print(retc)
mem = conn.getFreeMemory()
print('Free memory on the node is '+ str(mem) + ' bytes')
# get FRee pages is not implemented
buf = conn.getMemoryParameters()
for parm in buf:
    print(parm)
model = conn.getSecurityModel()
print("security model")
print(model[0] + "  "+ model[1])
print("sysinfo")
xmlinfo = conn.getSysinfo()
print(xmlinfo)
print("CPUMAP")
cpuMap = conn.getCPUMap()
print('cpus: '+ str(cpuMap[0]))
print('available: '+ str(cpuMap[1]))
print("cpustats")
cpuStats = conn.getCPUStats(0)
print(cpuStats)
print('getCPUmodelNames')
cpumodels = conn.getCPUModelNames('x86_64')
print(cpumodels)
dom = conn.lookupByName('first')
if dom == None:
    print('Error in opening domain')
print(dom)
# uuiddom = '44cb3358-6915-4a3e-87cd-a72cb233808d'
# dom1 = conn.lookupByUUID(uuiddom)
# print(dom1)
domainIDS = conn.listDomainsID() # returns list of ACTIVE domainIDs
if domainIDS == None:
    print('Error in getting domainIDs')
    exit(1)
if len(domainIDS) == 0:
    print('No active domains')
else:
    print(domainIDS)
allDomains = conn.listAllDomains(0)
name = []
if len(allDomains)!= 0:
    for domain in allDomains:
        print(domain.name())
        print('is Active: ' + str(domain.isActive()))
        print('isPersistent: ' + str(domain.isPersistent()))
        print('isUpdated: ' + str(domain.isUpdated()))
        print('Maxmemory: '+ str(domain.maxMemory()))
        name.append(domain.name())
for n in name:
    dom = conn.lookupByName(n)
    if dom != None:
        uuid = dom.UUIDString()
        print(uuid)
        print(dom.OSType())
        print(dom.hasCurrentSnapshot())
        print(dom.hasManagedSaveImage()) # running domain has no saved image
        print(dom.info())
        # print(dom.hostname()) # not supported

# example 4.7 not implemented\. looks obvious
network = conn.networkLookupByName('default')
print("virtual networks")
print(network.name())
print(network.bridgeName())
ifaceNames = conn.listInterfaces()
print('ifacenames')
for i in ifaceNames:
    print(i)
# iface = conn.interfaceLookupByName('ens3')
# print(iface.name())
# print(iface.MACString())
# dom = conn.lookupByName('first')
# if dom == None:
#     print("Failed to get the domain object.")
#     exit(1)
# iface = dom.interfaceAddresses(libvirt.VIR_DOMAIN_INTERFACE_ADDRESSES_SRC_AGENT,0)
# if iface == None:
#     print("Why")
#     exit(1)
# print(iface.iteritems())


# for n in network:
#     print(n)
conn.close()

exit(0)