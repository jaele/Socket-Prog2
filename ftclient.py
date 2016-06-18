#!/usr/bin/python

# Author: James Le
# Date: 5/25/2016
# Main Reference: https://docs.python.org/2/library/socketserver.html
# Main Reference: http://www.bogotobogo.com/python/python_network_programming_server_client_file_transfer.php
# Some Reference: http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html


import sys    # contains number of data types used in system calls
import socket # structures needed for sockets
import getopt
import os




def main():


	# initialize and declare variables for server name and port
	serverName = sys.argv[1]
	serverPort = int(sys.argv[2])
	fileName = ""
	command = ""
	serverDataPort = 0

	
	serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	serverSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	
	try:
		serverSocket.connect((serverName, int(serverPort)))
		print("Connected successfully.\n")
	
	except:
		print "Error connecting.\n"
		sys.exit()
	
	
	
	
	
	# if arg contains -l, send request for list
	if len(sys.argv) == 5:
		
		if(sys.argv[3] == "-l"):
			serverDataPort = sys.argv[4]
			command = "l" +  " " + serverDataPort
			print(command)
			
			serverSocket.send(command);
			
			try:
				# set up server with dataport
				serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				serverSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
				

				serverSocket.connect((serverName, int(serverDataPort)))
				print("Connected successfully.\n")
				
				# request for directory
				dirRequest(serverSocket)
	
				serverSocket.close()

			except:
				print "Error connecting.\n"
				sys.exit()
		
		
		
		else:
			print("Incorrect flag.\n")

		


	
	# if arg contains -g, send request for file transfer
	elif len(sys.argv) == 6:
		
		if(sys.argv[3] == "-g"):
			fileName = sys.argv[4]
			serverDataPort = sys.argv[5]
			command = "g" + " " + serverDataPort + " " + fileName
			print(command)

			serverSocket.send(command);
		
		
			try:
				# set up server with dataport
				serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				serverSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
				

				serverSocket.connect((serverName, int(serverDataPort)))
				print("Connected successfully.\n")

				# request for file
				fileRequest(serverSocket, fileName)
	
				serverSocket.close()

			except:
				print "Error connecting.\n"
				sys.exit()
		
		
		else:
			fileName = "invalidfile"
			print("Invalid filename.\n")

	# close socket
	serverSocket.close()
	




def dirRequest(serverDataSock):
	
	print "Requesting list directory.\n"
	directory = ''
	buffer = '\n'
	
	
	while buffer != '':
		buffer = serverDataSock.recv(1024)
		directory += buffer
	

	print(directory)
	
	print"Finished!\n"

	
	

# Reference: http://stackoverflow.com/questions/27241804/sending-a-file-over-tcp-sockets-in-python
# Reference: http://stackoverflow.com/questions/82831/how-to-check-whether-a-file-exists-using-python	
def fileRequest(serverDataSock, fileName):
	
	print "Requesting for file transfer.\n"
	
	for file in os.listdir('.'):
		if file == fileName:
			fileName += "Duplicate file name.\n"

			
	
	
	print "Now opening file to send..\n"
	contents = ''
	buffer = '\n'
	
	
	
	while buffer != '':
		print "Sending...\n"
		buffer = serverDataSock.recv(1024)
		contents += buffer
	
	
	
	with open(fileName, "w") as f:
		f.write(contents)
		
		print "Done sending.\n"


		
		
if __name__ == "__main__":
	main()
