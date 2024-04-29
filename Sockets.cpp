//Bind socket, you bind IP and port number to the socket itself
//Bound
//Once the server socket is bound you can pass it into the listen function



 //Server duplicates its socket which is creates a connected status
//You maintain the original listening socket
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

//for threading
#include <thread>

//for  file stream to send files (output file stream)
#include <fstream>





int main(int argc, char* argv[]) {
	//Server Side

	//Specify port number
	int port = 55555;




	//----------------------------------------------------------------------//
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
		std::cout << "Winsock dll not found" << std::endl;
		return 0;
	}
	else {
		std::cout << "Winsock dll found" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}

	//----------------------------------------------------------------------//
	//----------------------------------------------------------------------//


	//Create the Server Side Socket
	//This specifies the protocol to use, either TCP or UDP
	 //TCP socket
	//Create a socket variable and initialize it to invalid socket
	//Right now the socket is unbound
	SOCKET serverSocket;
	//Socket is initialized ot null
	serverSocket = INVALID_SOCKET;

	//call socket function passing int, TCP or UDP, socket type and the protocol
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


	//----------------------------------------------------------------------//
	//----------------------------------------------------------------------//




	//Bind the socket.
	//Bind the IP address and the port number to the created socket
	//We can use the bind function 
	// A server to specify which port it wants to bind the ip/port the socket to, clients do not, OS handles it
	//Will bind the data passed to it, to the socket and returns confirmation

	//create a socket address service variable to store ip, port and socket type
	sockaddr_in service;

	//in the TCP family
	service.sin_family = AF_INET;


	//Converts IP address to string binary
	InetPton(AF_INET, (L"192.168.87.22"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);

	//Passes in the whole sockaddr data structure, but type casts it first
	//bind function uses this information to bind to socket
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	else {
		std::cout << "bind() is OK!" << std::endl;
	}

	//----------------------------------------------------------------------//
	//----------------------------------------------------------------------//

	


	//Use the bounded function to listen for clients attempting to connect
	//to the socket on the server
	//2 parameters, the bounded unconnected socket and backlog which is the 
	//amount of clients that can connect at once
	//This is the server portion, it just waits here at the specified port for incoming connections
	//Waits for the connect() function
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		std::cout << "listen(): error listening on socket" << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "listen() is OK, I'm currently waiting for connections..." << std::endl;
	}



	//----------------------------------------------------------------------//
	//----------------------------------------------------------------------//



	//Accept functionality
	//Accept function pauses the execution of the server at this point
	//it will wait until the client has established a connection with the listening
	//socket, once that has been established, we have to accept the connection
	//Accept function is a blocking function
	//Once accepted, the accept function returns a new socket that is connected
	//to the client socket, the original listening socket is still listening
	//We need to create another socket variable to store the socket returned by the
	//accept function


	//For a file transfer implementation, we need to create a function to handles the 
	//connected client and will take the file
	//We can create a lambda function that will be passed into a thread with the accepted socket



	//Handle function which is a lambda function
	auto handleClient = [](SOCKET acceptSocket) {

		std::cout << "Entered thread" << std::endl;

		//Create a 1024mb buffer to recieve the file
		const int bufferSize = 1024;
		//create buffer
		char buffer[bufferSize];

		std::cout << "Buffer size pre recv " << sizeof(buffer) << std::endl;

		//Recieve the file data in chunks
		std::ofstream outputFile("received_file.txt", std::ios::binary);
		if (!outputFile) {
			std::cerr << "Failed to open file for writing" << std::endl;
			//close the socket to allow other resources to use it
			closesocket(acceptSocket);
			return;
		}

		//Create a variable bytes read, recv returns the number of bytes
		int bytesRead;
		
		//Create a for loop that writes the data until its fully written
		while ((bytesRead = recv(acceptSocket, buffer, bufferSize, 0)) > 0) {

			//write the data and save it to received_file.dat
			outputFile.write(buffer, bytesRead);
			std::cout << "Entered buffer write" << std::endl;
		}
		

		

		//close the output file
		outputFile.close();
		//close the socket
		closesocket(acceptSocket);
	};


	//Create a infinite while loop to represent as server
	//Create a thread to each connected client and detach it from the main thread
	//it will end since this is a infinite while loop

	while(true){
	SOCKET acceptSocket;
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
	std::cout << "going into thread" << std::endl;
	std::thread(handleClient, acceptSocket).detach();
	}



	//We will use the acceptedSocket to communicate w/ the client

	/*for (int i = 0; i < 1000; i++) {*/

		//RECEIEVE
		//Recieve data incoming from the client with recv
		//almost the same process
		//const int bufferSize = 200;
		//char recieveBuffer[bufferSize];



		//int byteCount = recv(acceptSocket, recieveBuffer, bufferSize, 0);
		//if (byteCount < 0) {
		//	std::cout << "Client: error " << WSAGetLastError;
		//	return 0;
		//}
		//else {
		//	//If its successful the recieve buffer will be the reply
		//	std::cout << "Client: " << recieveBuffer << std::endl;
		//}





		////SEND
		//const int sendbufferSize = 200;

		////create buffer
		//char sendbuffer[sendbufferSize];



		////Enter message to be sent to server/client
		//std::cout << "Enter your message:";

		////Store message in buffer
		//std::cin.getline(sendbuffer, sendbufferSize);

		////use the send function to send out the message, and do error handling
		////send returns an int of the amount of bytes sent
		//int sendbyteCount = send(acceptSocket, sendbuffer, sendbufferSize, 0);
		//if (sendbyteCount == SOCKET_ERROR) {
		//	std::cout << "Sever send error: " << WSAGetLastError() << std::endl;
		//	return -1;
		//}
		//else {
		//	/*std::cout << "Success! Client received: " << sendbyteCount << " bytes of data.." << std::endl;*/


		//}

	/*}*/


	

	system("pause");
	//Close the socket
	closesocket(serverSocket);
	//Clean up the socket after program is done
	WSACleanup();




	






	
	return 0;
}