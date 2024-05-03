//Server side impl

//Includes which store the necessaray libraries 

#include <WinSock2.h>
//For inetpton
#include <ws2tcpip.h>
#include <iostream>
//for threading
#include <thread>
//for  file stream 
#include <fstream>



//Handle function
//Handle function which is a lambda function
//Auto to represent lambda
auto handleClient = [](SOCKET acceptSocket) {

	std::cout << "Entered thread" << std::endl;

	//Create a 1024mb buffer to recieve the file
	const int bufferSize = 1024;
	//create buffer
	char buffer[bufferSize];

	//Recieve the file data in chunks
	//Create an output file, and writes the file in binary
	std::ofstream outputFile("recieve.MOV", std::ios::binary);
	if (!outputFile) {
		std::cerr << "Failed to open file for writing" << std::endl;
		//close the socket to allow other resources to use it
		closesocket(acceptSocket);
		return;
	}
	else {
		std::cout << "Created a new output file to write to database " << std::endl;
	}

	//Create a variable bytes read, 
	//recv returns the number of bytes
	int bytesRead;
	int totalBytesWritten = 0;

	//Create a for loop that writes the data until its fully written
	while ((bytesRead = recv(acceptSocket, buffer, bufferSize, 0)) > 0) {

		//write the data and save it to the file
		//Reading from the buffer, and the return value from bytesRead
		outputFile.write(buffer, bytesRead);

		//Gets a counter of the total amount of bytes read
		totalBytesWritten += bytesRead;
		std::cout << "Bytes Read: " << totalBytesWritten << std::endl;
	}

	//close the output file
	outputFile.close();
	//close the socket
	closesocket(acceptSocket);
};






int main(int argc, char* argv[]) {

	//Specify port number
	int port = 55555;

	
	//----------------------------------------------------------------------//
	// Initialize the Window Socket DLL
	//----------------------------------------------------------------------//


	//Initialize the Winsock DLL
	//Create a wsaData variable to pass into WSAStartup
	WSADATA wsaData;

	//WSA method will return an int for if its successful or not
	int wsaerr;

	//format the version requested parameter as a WORD variable
	WORD wVersionRequested = MAKEWORD(2, 2);

	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr) {
		std::cout << "Winsock dll was not found" << std::endl;
		return 0;
	}
	else {
		std::cout << "Winsock dll was found" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}



	//----------------------------------------------------------------------//
	// Create the socket
	//----------------------------------------------------------------------//


	//This specifies the protocol to use, either TCP or UDP
	//We are going to create a TCP socket
	//Create a socket variable and initialize it to invalid socket
	//Right now the socket is unbounded

	SOCKET serverSocket;

	//Socket is initialized to null
	serverSocket = INVALID_SOCKET;

	//call socket function passing int, which signifies if it TCP or UDP, 
	//socket type is for the TYPE of socket, sock stream is TCP, DGRAM is UDP
	//and the protocol, TCP is IPPROTO_TCP, UDP is IPPROTO_UDP
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Error checking to see if the socket was successfully created
	if (serverSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		//if there is an error, clean up the the library
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "Socket() is OK" << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}


	//----------------------------------------------------------------------//
	// Binding the socket to an IP/PORT
	//----------------------------------------------------------------------//


	//Bind the IP address and the port number to the created socket
	//We can use the bind function 
	//A server to specify which port it wants to bind the ip/port the socket to, clients do not, OS handles it
	//Will bind the data passed to it, to the socket and returns confirmation
	//Bind socket, you bind IP and port number to the socket itself
	//Bound
	//Once the server socket is bound you can pass it into the listen function
	

	//create a socket address service variable to store ip, port and socket type
	sockaddr_in service;

	//in the TCP family
	service.sin_family = AF_INET;

	//Converts IP address to string binary
	InetPton(AF_INET, (L"192.168.87.22"), &service.sin_addr.s_addr);

	//htons is host to network, host is typically little endian byte order
	//and this converts it to big-endian which is network byte order
	service.sin_port = htons(port);

	//Passes in the whole sockaddr data structure, but type casts it first
	//bind function uses this information to bind to socket
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed to bind: " << WSAGetLastError() << std::endl;
		//close the socket
		closesocket(serverSocket);
		//clean up
		WSACleanup();
		//return error code
		return -1;
	}
	else {
		std::cout << "Server socket has successfully binded to IP/PORT" << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}

	//----------------------------------------------------------------------//
	// Server socket will listen on specified port number
	//----------------------------------------------------------------------//

	
	//Use the listen function to listen for clients attempting to connect
	//to the bounded socket on the server
	//2 parameters, the bounded unconnected socket and backlog which is the 
	//amount of clients that can connect at once
	//This is the server portion, it just waits here at the specified port for incoming connections
	// It will be in the listening state for the duration of the program, or until the socket is closed
	//Waits for the connect() function
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		std::cout << "listen (): error listening on socket" << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "Server Socket is listening, currently waiting for incoming connections..." << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}


	//----------------------------------------------------------------------//
	// Accept function, Creates a new socket
	//----------------------------------------------------------------------//


	//Accept functionality
	//Accept function pauses the execution of the server at this point
	//it will wait until the client has established a connection (with connect())with the listening
	//socket (server socket), once that has been established, we have to accept the connection
	//Accept function is a blocking function
	//Once accepted, the accept function returns a new socket that is connected
	//to the client socket, the original listening socket is still listening
	//We need to create another socket variable to store the socket returned by the
	//accept function

	//Server duplicates its socket 
	//You maintain the original listening socket, which is still listening on the specified port number

	//For a file transfer implementation, we need to create a function to handle the 
	//connected client, which will recieve files
	//We can create a lambda function that will be passed into a thread with the accepted socket
	//A thread is created so we can create a subprocess to handle files while continuing with the program

	//Create a handle client function which will recieve files from a client

	


	//Create a infinite while loop to represent a server
	//Create a thread to each connected client and detach it from the main thread

	while(true){
	//Create a new socket that will be used to communicate with the client socket
	SOCKET acceptSocket;

	//While the serverSocket is in the listening state, once a connection connects to the listening socket
	//the accept function will be able to create a new socket from that
	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "accept failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}
	else {
		/*std::cout << "Accept Success!" << std::endl;*/
		std::cout << "A client has connected to the Server." << std::endl;
	}

	//Create a thread to run the lambda function
	//pass in the function and the socket 
	//detach it from main method
	std::thread(handleClient, acceptSocket).detach();
	}


	system("pause");
	//Close the socket
	closesocket(serverSocket);
	//Clean up the socket after program is done
	WSACleanup();

	return 0;
}