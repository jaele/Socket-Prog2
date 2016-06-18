# Socket-Prog2
Creating a client and server file transfer system


James Le
CS 372

Steps:
1. Put ftserver.cpp in a folder directory on a server
2. Put the file hello in the same folder directory as ftserver.cpp
3. Put ftclient.py in a different folder directory on the same server
4. Open up two command terminals in Putty, both on flip1.engr.oregonstate.edu server
5. Compile ftserver by typing in g++ -o ftserver ftserver.cpp


Next Steps:
1. ftserver will start on Host A and waits for client on a portnum
	- ./ftserver <server_port> or 
	- ./ftserver 5000

2. ftclient will start on Host B and sends command line to Host A
	- python ftclient.py <server_name> <server_port> <command> <data_port> or
	- python ftclient.py flip1.oregonstate.edu 5000 -l 5001  // to get list directory from HostA

	- python ftclient.py <server_name> <server_port> <command> <filename> <data_port> or
	- python ftclient.py flip1.oregonstate.edu 5000 -g hello 5001  // to get file hello from HostA
