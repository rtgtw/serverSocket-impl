//Bind socket, you bind IP and port number to the socket itself
//Bound
//Once the server socket is bound you can pass it into the listen function



 //Server duplicates its socket which is creates a connected status
//You maintain the original listening socket
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>




int main(int argc, char* argv[]) {
//Server Side

//Specify port number
	int port = 55555;

//Initialize the DLL
//Create a wsaData variable to pass into WSAStartup
	WSADATA wsaData;

//WSA method will return an int for if its successful or not
	int wsaerr;

	//format the version requested parameter as a WORD variable
	WORD wVersionRequested = MAKEWORD(2, 2);

	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr) {
		std::cout << "Winsock dll not found" << std::endl;
		return 0;
	}
	else {
		std::cout << "Winsock dll found" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}



	//Create the Server Side Socket
	//This specifies the protocol to use, either TCP or UDP
	 //TCP socket
	//Create a socket variable and initialize it to invalid socket
	//Right now the socket is unbound
	SOCKET serverSocket;
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		//if there is an error, clean up the the library
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "Socket() is OK" << std::endl;
	}

	


	//Bind the socket.
	//Bind the IP address and the port number to the created socket
	//We can use the bind function 
	// A server to specify which port it wants to bind the ip/port the socket to, clients do not, OS handles it
	//Will bind the data passed to it, to the socket and returns confirmation

	//create a socket address service variable to store ip, port and socket type
	sockaddr_in service;
	service.sin_family = AF_INET;

	//Converts IP address to string binary
	InetPton(AF_INET, (L"127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	
	//Passes in the whole sockaddr data structure, but type casts it first
	//bind function uses this information to bind to socket
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		std::cout << "bind() is OK!" << std::endl;
	}


	//Use the bounded function to listen for clients attempting to connect
	//to the socket on the server
	//2 parameters, the bounded unconnected socket and backlog which is the 
	//amount of clients that can connect at once
	//This is the server portion, it just waits here at the specified port for incoming connections
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		std::cout << "listen(): error listening on socket" << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "listen() is OK, I'm currently waiting for connections..." << std::endl;
	}



	//Accept functionality
	//Accept function pauses the execution of the server at this point
	//it will wait until the client has established a connection with the listening
	//socket, once that has been established, we have to accept the connection
	//Accept function is a blocking function
	//Once accepted, the accept function returns a new socket that is connected
	//to the client socket, the original listening socket is still listening
	//We need to create another socket variable to store the socket returned by the
	//accept function
	SOCKET acceptSocket;
	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "accept failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}
	else {
		std::cout << "Accept Success!" << std::endl;
		std::cout << "A client has connected to the Server." << std::endl;
	}



	


	//Close the socket
	closesocket(serverSocket);
	//Clean up the socket after program is done
	WSACleanup();
	
	return 0;
}