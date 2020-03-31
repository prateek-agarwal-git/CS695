#!/bin/bash
scp rc-local.service prateek@192.168.122.21:~/rc-local.service
scp rc-local.service prateek@192.168.122.22:~/rc-local.service 
scp rc-local.service prateek@192.168.122.23:~/rc-local.service 
scp rc-local.service prateek@192.168.122.24:~/rc-local.service 
scp rc-local.service prateek@192.168.122.25:~/rc-local.service 

scp rc.local prateek@192.168.122.21:~/rc.local
scp rc.local prateek@192.168.122.22:~/rc.local 
scp rc.local prateek@192.168.122.23:~/rc.local 
scp rc.local prateek@192.168.122.24:~/rc.local 
scp rc.local prateek@192.168.122.25:~/rc.local 

# ssh prateek@192.168.122.22 -t sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service  
# ssh prateek@192.168.122.23 -t sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service 
# ssh prateek@192.168.122.24 -t sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service 
# ssh prateek@192.168.122.25 -t sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service 


ssh prateek@192.168.122.21  -t  " sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service; sudo cp ~/rc.local /etc/rc.local; sudo systemctl disable rc-local; sudo systemctl enable rc-local"
ssh prateek@192.168.122.22  -t  " sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service; sudo cp ~/rc.local /etc/rc.local; sudo systemctl disable rc-local; sudo systemctl enable rc-local"
ssh prateek@192.168.122.23  -t  " sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service; sudo cp ~/rc.local /etc/rc.local; sudo systemctl disable rc-local; sudo systemctl enable rc-local"
ssh prateek@192.168.122.24  -t  " sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service; sudo cp ~/rc.local /etc/rc.local; sudo systemctl disable rc-local; sudo systemctl enable rc-local"
ssh prateek@192.168.122.25  -t  " sudo cp ~/rc-local.service /etc/systemd/system/rc-local.service; sudo cp ~/rc.local /etc/rc.local; sudo systemctl disable rc-local; sudo systemctl enable rc-local"



# ssh prateek@192.168.122.21  -t sudo cp ~/rc.local /etc/rc.local 
# ssh prateek@192.168.122.22 -t sudo cp ~/rc.local /etc/rc.local
# ssh prateek@192.168.122.23 -t sudo cp ~/rc.local /etc/rc.local
# ssh prateek@192.168.122.24 -t sudo cp ~/rc.local /etc/rc.local
# ssh prateek@192.168.122.25 -t sudo cp ~/rc.local /etc/rc.local
