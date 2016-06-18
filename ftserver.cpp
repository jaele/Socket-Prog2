/****************************************************************************************
** Author: James Le
** Date: 4/28/2016
** Main Reference: http://www.linuxhowtos.org/C_C++/socket.htm
** Some Reference: http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
*****************************************************************************************/


#include <stdio.h>  // contains declarations used in most input and output
#include <sys/types.h> // contains number of data types used in system calls
#include <sys/socket.h> // structures needed for sockets
#include <netinet/in.h>  // structures needed for internet domain addresses
#include <string.h>     // for string inputs
#include <stdlib.h>  // standard library
#include <unistd.h>  // access POSIX for various constant, type, and function declarations
#include <iostream>  // defines standard input/output stream objects
#include <fcntl.h>   // use for directory
#include <dirent.h>  // use for directory
#include <vector>   // for using vectors


void startChat(int);
void sendListDir(int);
void sendFile(int, std::string);

int ff = 0;

int main(int argc, char *argv[])
{

	/* Variables */

	int sockFd;	// socket for listening
	int mySock;	// socket for connecting
	int portNum = atoi(argv[1]);  // port number taking arg from command line

	struct sockaddr_in server, client;  // server and client address, defined in netinet
	socklen_t clilen;   // stores size of address of client
	
	
	/* Create socket */

	// socket() system call creates a new socket, and takes three arguments
	// the first is the domain address, the second is the type of socket, the third is the protocal (always 0)
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd < 0) 
	{
		perror("Error opening socket");
		exit(1);  // exit program
	}

	// set fields for server
	server.sin_family = AF_INET;  // contains code for the address family
	server.sin_addr.s_addr = INADDR_ANY;  // contains IP address of the host
	server.sin_port = htons(portNum);  // contains the port number
	
	
	/* Call bind */ 

	// bind() system call binds a socket to an address
	// the address of the current host and port number on which the server will run
	if (bind(sockFd, (struct sockaddr *)&server, sizeof(server)))
	{
		perror("Bind failed");  // binding will fail if address is already in use
		exit(1);
	}

	
	/* Listen */

	// listen() system call allows the process to listen on the socket for connections
	// 5 is max value on many systems
	listen(sockFd, 5);
	
	std::cout<<"Server opened on port " << portNum << std::endl;
	std::cout<< "Waiting for client to connect.." << std::endl;
	
	
	/* Access  */

	// accept() system call causes the process to block until a client connects to the server
	// this loops keeps the server running until it gets a sigint
	do 
	{ 
		// creates new thread for chat
		clilen = sizeof(client);
		mySock = accept(sockFd, (struct sockaddr *) &client, &clilen);
		
		std::cout<<"Client has connected. " << std::endl;
		
		char buffer[512];
		char fileName[512];
		char dataPortStr[16];
		char type[4];
		int dataPortNo;
		
		// http://www.cplusplus.com/reference/cstring/memset/?kw=memset
		// use for pointing to specified char values below
		memset(buffer, '\0', 512);
		memset(type, '\0', 4);
		memset(fileName, '\0', 512);
		memset(dataPortStr, '\0', 16);
		
		int rVal;
		rVal = recv(mySock, buffer, 512, 0);
		
		
		if(mySock == -1)
		{
			perror("Accept failed");
		}
		
		// http://www.cplusplus.com/reference/cstring/strtok/?kw=strtok
		// split command lines into tokens
		std::cout <<"Command lines: " << buffer << std::endl;
		char *temp;
		temp = strtok(buffer, " \n\0");
		strcpy(type, temp);
		
		std::cout<<"Command type (l/g): " << type << std::endl;
		
		temp = strtok(NULL, " \n\0");
		strcpy(dataPortStr, temp);
		
		dataPortNo = atoi(temp);
		
		if(type[0] == 'g') 
		{
			std::cout<<"Getting the filename.. " << std::endl;
			temp = strtok(NULL, " \n\0");
			strcpy(fileName, temp);
		}
		
		
		if (type[0] == 'l')
		{
			std::cout<<"Calling the list directory.. " << std::endl;
			sendListDir(dataPortNo);
		}
		
		
		else if(type[0] == 'g') 
		{
			std::cout<<"calling get command " << std::endl;
			std::cout<< "dataportnum: " << dataPortNo << " filename: " <<fileName << std::endl;
			sendFile(dataPortNo, fileName);
		}
	
		else
		{
			std::cout<<"error sending error message" << std::endl;
			exit(0);
		}
		
		
	} while(1);

	// close sockets
	close(sockFd);
	close(mySock);

return 0;
}


void sendListDir(int port)
{
	int sockFd;	// socket for listening
	int mySock;	// socket for connecting
	int rVal;
	
	/* Variables */

	
	struct sockaddr_in server, client;  // server and client address, defined in netinet
	socklen_t clilen;   // stores size of address of client
	
	
	/* Create socket */

	// socket() system call creates a new socket, and takes three arguments
	// the first is the domain address, the second is the type of socket, the third is the protocal (always 0)
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd < 0) 
	{
		perror("Error opening socket");
		exit(1);  // exit program
	}

	// set fields for server
	server.sin_family = AF_INET;  // contains code for the address family
	server.sin_addr.s_addr = INADDR_ANY;  // contains IP address of the host
	server.sin_port = htons(port);  // contains the port number
	
	
	/* Call bind */ 

	// bind() system call binds a socket to an address
	// the address of the current host and port number on which the server will run
	if (bind(sockFd, (struct sockaddr *)&server, sizeof(server)))
	{
		perror("Bind failed");  // binding will fail if address is already in use
		exit(1);
	}

	
	/* Listen */
	
	// listen() system call allows the process to listen on the socket for connections
	// 5 is max value on many systems
	listen(sockFd, 5);
	
	
	clilen = sizeof(client);
	mySock = accept(sockFd, (struct sockaddr *) &client, &clilen);
	
	
	if(mySock == -1)
		{
			perror("Accept failed");
		}
	
	// http://www.cplusplus.com/forum/unices/16005/
	// http://www.cplusplus.com/forum/beginner/167529/
	std::cout<<"Sending directory contents on port " << port << std::endl;
	char sentMessage[512];
	bzero(sentMessage,512);
	
	
	std::string dir = std::string(".");
	std::vector<std::string> files = std::vector<std::string>();
	
	DIR *dp;  // find dir information
	struct dirent *dirp;  // create a struct
	
	if((dp = opendir(dir.c_str())) == NULL) 
	{
		std::cout << "Error" << std::endl;
	}
	
	
	while((dirp = readdir(dp)) !=NULL)
	{
		files.push_back(std::string(dirp->d_name));
		sprintf(sentMessage, "%s\n%s ", sentMessage, dirp->d_name); // write message
	}
	
	closedir(dp);
	
	
	rVal = write(mySock, sentMessage, strlen(sentMessage));
	close(sockFd);
	close(mySock);
	
	
}


void sendFile(int port, std::string fileName)
{
	int sockFd;	// socket for listening
	int mySock;	// socket for connecting
	int rVal;
	
	/* Variables */

	
	struct sockaddr_in server, client;  // server and client address, defined in netinet
	socklen_t clilen;   // stores size of address of client
	
	
	/* Create socket */

	// socket() system call creates a new socket, and takes three arguments
	// the first is the domain address, the second is the type of socket, the third is the protocal (always 0)
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd < 0) 
	{
		perror("Error opening socket");
		exit(1);  // exit program
	}

	// set fields for server
	server.sin_family = AF_INET;  // contains code for the address family
	server.sin_addr.s_addr = INADDR_ANY;  // contains IP address of the host
	server.sin_port = htons(port);  // contains the port number
	
	
	/* Call bind */ 

	// bind() system call binds a socket to an address
	// the address of the current host and port number on which the server will run
	if (bind(sockFd, (struct sockaddr *)&server, sizeof(server)))
	{
		perror("Bind failed");  // binding will fail if address is already in use
		exit(1);
	}

	
	/* Listen */
	
	// listen() system call allows the process to listen on the socket for connections
	// 5 is max value on many systems
	listen(sockFd, 5);
	
	
	clilen = sizeof(client);
	mySock = accept(sockFd, (struct sockaddr *) &client, &clilen);
	
	// http://www.linuxhowtos.org/manpages/2/write.htm
	if(mySock == -1)
		{
			perror("Accept failed");
		}
		
		
		char buffer[512];
		
		FILE *fs = fopen(fileName.c_str(), "r"); // open the fileName
		if(fs == NULL)
		{
			std::cout<<"File not found. Sending error message" << std::endl;
			ff = 1;
			exit(1);
		}
		
		
		std::cout<<"Sending " << fileName << " to port " << port << std::endl;
		
		bzero(buffer, 512);
		int fs_block_sz;
		while((fs_block_sz = fread(buffer, sizeof(char), 512, fs)) > 0)
		{
			if(send(mySock, buffer, fs_block_sz, 0) < 0)
			{
				break;
			}
			
			bzero(buffer, 512);
		}
		
		std::cout<<"File " << fileName << " has been sent!" << std::endl;
		
		close(sockFd);
		close(mySock);
	
}


