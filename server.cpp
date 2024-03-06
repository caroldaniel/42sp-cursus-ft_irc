// C++ program to show the example of server application in 
// socket programming 
#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

using namespace std; 

// IPV4 

// teste com nc 
// nc 127.0.0.1 8080 ou nc localhost 8080

int main(int argc, char *argv[]) 
{ 
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // creating socket 

	sockaddr_in serverAddress; 
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080); 
	serverAddress.sin_addr.s_addr = INADDR_ANY; // specifying the address

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); // binding socket. 

	listen(serverSocket, 5); // listening to the assigned socket  

	while (true)
	{
		cout << "Waiting client connection..." << endl;
		int clientSocket = accept(serverSocket, nullptr, nullptr); // accepting client connection 
		cout << "Client connected" << endl;
		char buffer[1024] = {0}; 
		ssize_t bytesRead;

		do
		{
			bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0); // receiving client message
			if (bytesRead > 0) {
				cout << "from client: " << buffer << endl;  // printing client message
				memset(buffer, 0, sizeof(buffer)); // clear buffer
			}
		} while (bytesRead > 0); // keep executing while has receiving bytes from the client

		close(clientSocket);
		cout << "Client disconnected" << endl;
	}
	close(serverSocket); 

	return 0; 
}
