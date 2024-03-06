[Reference](https://www.geeksforgeeks.org/socket-programming-in-cpp/)

# What is a socket and how can use it? 
 A socket is a communication endpoint that allows processes to communicate over a network, typically using the Internet Protocol (IP).

# Creating a socket
### **int socket(int domain, int type, int protocol)**

**The socket() function returns a new socket descriptor (a small non-negative integer) on success, or -1 for errors, which can be used for further socket operations such as binding the socket to a specific address and port (next step), listening for incoming connections, connecting to a remote host, sending and receiving data, etc.**

int domain: specifies the communication domain in which the socket will be used.
AF_INET: IPv4 communication domain.

int Type: specifies the type of socket to be created, which determines the semantics of communication:
SOCK_STREAM: Provides sequenced, reliable, two-way, connection-based byte streams. It uses TCP (Transmission Control Protocol) for communication, which ensures that data arrives in the same order as sent and without errors.

int protocol: specifies the specific protocol to be used with the socket. Usually, this argument is set to 0, which indicates that the system should choose the appropriate protocol based on the combination of the specified domain and type.

# Binding a socket
 With success to create a new FD to our server socket we need to binding the socket to a specific address using bind() function

### int bind(int fd, const sockaddr *addr, socklen_t len)

**The bind() function associates the specified address with the socket identified by fd. This is primarily necessary for servers, where the socket needs to specify the address and port on which it will listen for client connections. It's important to note that the address passed to bind() must be compatible with the socket type and address family specified during the socket creation. For example, if you create a socket with AF_INET (IPv4) and SOCK_STREAM (TCP), then you should pass a sockaddr_in structure containing an IPv4 address to bind().**

int fd: This is the file descriptor of the socket to which you want to associate an address.

const sockaddr *addr: This argument is a pointer to a sockaddr structure that contains the address you want to associate with the socket. By default, sockaddr is a generic structure that can be used to represent addresses from different protocol families. However, you typically use sockaddr_in for IPv4 addresses or sockaddr_in6 for IPv6 addresses, depending on the address family used when creating the socket.
example using ipv4:

```
struct sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET; // IPV4
serverAddress.sin_port = htons(8080); // Port number 8080
serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections on any available interface
```

socklen_t len: This argument specifies the size of the address passed in "addr", in bytes. You typically use the sizeof() function to calculate this value. // sizeof(serverAddress)

# Using a socket 
 After creating a socket and binding it, a communication can be established by listening and accepting a new client. There is below an example of code to test a socket connection:

```
#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

using namespace std; 

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


```
After compiling and running the code above, you can send a message with the command nc in your terminal
**"nc 127.0.0.1 8080"**
or, if you prefer, compile and run this other code representing a client.
```

#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
  
int main() 
{ 
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
  
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 
  
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); 
  
    const char* message = "Hello"; 
    send(clientSocket, message, strlen(message), 0); 
  
    close(clientSocket); 

    return 0; 
}
```

In these examples, you can see functions such as listen(), accept(), connect(), send(), close(), recv(), htons() in use, but they are simple functions that often already have a self-explanatory name for what they do, and don't need a profound explanation.
