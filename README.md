<h1 align=center>
	<b>ft_irc</b>
</h1>

<h2 align=center>
	 <i>42cursus' project #13</i>
</h2>

<p align=center>
	This is the second last project of the 42cursus' common core. The goal of this project is to create an IRC server in C++98. Now, we should be able to study protocols, open sockets, and handle multiple connections at the same time. This project is a great opportunity to learn about network programming and to understand how servers and clients communicate with each other. Does it sound simple? Well, it's not. But, for me at least, was a lot of fun.


---
<div align=center>
<h2>
	Final score
</h2>
<img src="" alt="cado-car's 42Project Score"/>
<h4>Completed + Bonus</h4>
<img src="" alt="cado-car's 42Project Badge"/>
</div>

---

<h3 align=center>
Mandatory
</h3>

> <i> For the <b>mandatory</b> part, you must follow these rules: </i>
> - You must implement an IRC server in C++98;
> - You mustn't develop a client, but must choose an existing one as reference;
> - You mustn't handle server-server communication;
> - The server must be able to handle several clients at the same time, and all communication must be done via TCP/IP (v4 or v6);
> - Forking is not allowed;
> - All I/O operations must be non-blocking;
> - Only 1 poll() is allowed;
> - You must be able to authenticate, set a nickname, a username, join a channel, send and receive private messages;
> - You must handle the following commands: KICK, INVITE, TOPIC, MODE (with i, t, k, o and l parameters);


<h3 align=center>
Bonus
</h3>

> <i> For the <b>bonus</b> part, you must follow these rules: </i>
> - Handle file transfers;
> - A bot;

---

<h2>
The project
</h2>

So, like in every single project so far, the real question here is: where do we start? 

I think a good way to approach this project is to try and understand everything we can about servers and clients. How to connect them, how to make them interact with each other and, most importantly, what types of "rules" these connections follow - also known as protocols.

### What is a server?

In simple terms, a **server** is a computer program or a device that provides any sort of service to other programs or devices, known as **clients**. These services can be anything from sharing data or resources to performing computations for the client. APIs, for example, are a type of server that provides a set of rules for how software programs can interact with each other. Databases are also servers, as they provide a service to store and retrieve data.

One important thing to have in mind is that servers are not necessarily on the same machine as the client. They can be located anywhere in the world, as long as they have ways to establish a connection with the client.

### How to connect a client to a server?

The most common way to connect a client to a server is through the **TCP/IP** protocol. This particular protocol is the most widely used protocol in the world, and it governs how data is sent and received over the internet. 

It's not for us, in this project, to understand in depth how TCP/IP works. However, it is important to understand what protocols are, and how they work. 

### What is a protocol?

A **protocol** is a set of rules that govern how data is transmitted between devices in a network. It's like a language that is used to allow communication. In order to establish a connection, both the client and the server must speak the same language. 

Protocols are conventions. Most of them, today, are standardized, which means that they are widely accepted and used. We can find protocols for almost everything, from how to send an email to how to connect to a server. 

### What is RFC?

The **Request for Comments** (RFC) documents are a series (with thousands of entries) of documents that describe the protocols, technologies, and policies used in the internet. They are the result of the work of the Internet Engineering Task Force (IETF), and they are the standardization documents of the internet.

All (or almost all) protocols used in the internet are described in RFC documents. They are, for lack of a better word, the "bible" of the internet.

So, that means that if you want (and you do want) to create a server from scratch - and in our case, an IRC server, then you should probably start by reading the RFC documents that describe the IRC protocol. To be more precise, the **RFC 1459** and the **RFC 2810 to 2813**.

Again, it's not for us to understand in depth how these protocols work. But it's important to understand that they exist, and that they are the rules that we must follow in order to create our IRC server (especially because we are going to be able to connect to a real IRC client in the process).

### What is an IRC server?

But after all, what in the world is an IRC server?

The **Internet Relay Chat** (IRC) is a protocol that allows users to communicate with each other in real-time. It's a text-based protocol, and it's been around since 1988. It's one of the oldest protocols still in use today.

With an IRC server, users must be able to connect to it, and then they can join channels, send private messages, and perform a series of other actions. The server is responsible for handling all these actions, and for sending the messages to the right clients, either in channels, privately, or to the server itself, in order to perform some action.

That's what we are going to develop the logic behind the IRC server's resposes to the clients' actions. How to store the users' information, how to handle the channels, and how to send and receive messages, either in channels or privately.

But first, we must understand how to create a server in C++.

<h2>
Creating a server in C++
</h2>

The first real technical challenge of this project is to create a server in C++. 

For starters, we must be able to connect two programs through a network. This is done through **sockets**.

### What is a socket?

A **socket** is one endpoint of a two-way communication link between two programs running on the network. A socket is bound to a port number so that the TCP layer can identify the application that data is destined to. Sounds complicated? Surprisingly, it's not.

Imagine that a socket is like a telephone. When you want to call someone, you pick up the phone and dial the number. The other person picks up the phone and answers. The connection is established, and you can start talking.

In the same way, when you - a client - want to connect to a server, you create a socket and connect it to the server's address and port. The server accepts the connection, and the connection is established. You can start sending and receiving data.

A socket is the combination of an IP address and a port number. The IP address is the address of the server, and the port number is the "door" that you want to knock on - or the number of the extension that you want to call.

The same way, the client creates a socket and connects it to the server's address and port, the server creates a socket and binds it to a port. When a client connects to the server, the server accepts the connection and creates a new socket to communicate with the client. 

Since we won't be creating a client in this project, we are going to focus on the server's side of the connection. But, either way, we will need to extract the client's address and port in order to send and receive data correctly.

### How to create sockets? 

Regardless of the programming language, creating a socket is a relatively simple process, and it's supported by most operating systems.

In C++, we can create a socket using the **socket()** function, which is part of the **sys/socket.h** library. This function returns a file descriptor that we can use to send and receive data.

This is important to understand: under the hood, a socket is a file descriptor. This means that we can use the same functions that we use to read and write files to send and receive data through a socket. 

When calling the right functions, the operating system takes care of the rest. The kernel will be the one responsible for handling the data that is sent and received through the socket. It's the kernel that sends the data to the network, and it's the kernel that receives the data from the network. Our program only needs to read and write to the correct file descriptor.

In C++, we will need to use the following functions to enable the full capabilities of the socket:

- **`socket()`**: to create the socket; 
- **`bind()`**: to bind the socket to a port;
- **`listen()`**: to listen for incoming connections;
- **`accept()`**: to accept an incoming connection;

Each one of these functions has its own parameters and return values, and we must fully understand how to use them.

#### The `socket()` function

```c++
int socket(int domain, int type, int protocol);
```

The `socket()` function creates a new socket and returns a file descriptor that we can use to send and receive data. 

- The `domain` parameter specifies the protocol family that we want to use. In our case, we are going to use the `AF_INET` protocol family, which is the most common protocol family used in the internet. This protocol family is used with the `IPv4` addresses. If you want to use the `IPv6` protocol family, you should use the `AF_INET6` protocol family. The macros for these protocol families are defined in the `sys/socket.h` library.

- The `type` parameter specifies the type of the socket. We are going to use the `SOCK_STREAM` type, which is a reliable two-way connection-based byte stream. This means that the data is sent and received in the same order that it was sent, and that the data is not lost or duplicated. If you want to use a connectionless datagram service, you should use the `SOCK_DGRAM` type.

- The `protocol` parameter specifies the protocol that we want to use. We are going to use the `0` protocol, which is the default protocol for the given domain and type. If you want to use a specific protocol, you should use the `getprotobyname()` function to get the protocol number.

After calling the `socket()` function, we will have a file descriptor that we can use to send and receive data.

#### The `bind()` function

```c++
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

Now, we need to bind the socket to a port. That's the difference between a regular socket and a server socket. A regular socket is not bound to any port, and it's not listening for incoming connections. A server socket is bound to a port, and it's listening for incoming connections.

In clients, for example, we don't need to bind the socket to a port. We only need to connect the socket to the server's address and port - we usually use the function `connect()` to do that.

In servers, however, we need to bind the socket to a port. That means that the server will be able to listen for incoming connections on that port.

The `bind()` function takes three parameters:

- The `sockfd` parameter is the file descriptor of the socket that we want to bind to a port;

- The `addr` parameter is a pointer to a `sockaddr` structure that contains the address and port that we want to bind the socket to. This structure is a generic structure that can be used with any protocol family. In our case, we are going to use the `sockaddr_in` structure, which is specific to the `AF_INET` protocol family. This structure contains the `sin_family` member, which is the protocol family, the `sin_port` member, which is the port number, and the `sin_addr` member, which is the address. The `sin_addr` member is usually set to `INADDR_ANY`, which means that the socket will be bound to all available interfaces. The `sin_port` member is set to the port number that we want to bind the socket to. Both of these members must be in network byte order, which means that we must use the `htons()` function to convert the port number to network byte order.

- The `addrlen` parameter is the size of the `addr` parameter. This parameter is usually set to `sizeof(struct sockaddr_in)`.

After calling the `bind()` function, the socket will be bound to the specified port. However, it's not yet listening for incoming connections. 

#### The `listen()` function

```c++
int listen(int sockfd, int backlog);
```

Now, we need to listen for incoming connections. That means that the server will be able to accept incoming connections on the specified port.

The `listen()` function takes two parameters:

- The `sockfd` parameter is the file descriptor of the socket that we want to listen for incoming connections;

- The `backlog` parameter is the maximum length of the queue of pending connections. This parameter is usually set to `5`. You can even set it to any value that you want, but most operating systems will silently truncate it to `5`.

After calling the `listen()` function, the socket will be listening for incoming connections. However, it's not yet accepting incoming connections.

#### The `accept()` function

```c++
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

Finally, we need to accept incoming connections. That means that any client that attempts to connect to the server will be able to do so.

The `accept()` function takes three parameters:

- The `sockfd` parameter is the file descriptor of the socket that we want to accept incoming connections;

- The `addr` parameter is a pointer to a `sockaddr` structure that will contain the address and port of the client that is connecting to the server. This structure is a generic structure that can be used with any protocol family. In our case, we are going to use the `sockaddr_in` structure, which is specific to the `AF_INET` protocol family. This structure contains the `sin_family` member, which is the protocol family, the `sin_port` member, which is the port number, and the `sin_addr` member, which is the address. The `sin_port` member is set to the port number of the client that is connecting to the server. The `sin_addr` member is set to the address of the client that is connecting to the server. Both of these members will be in network byte order, which means that we must use the `ntohs()` function to convert the port number to host byte order, and the `inet_ntoa()` function to convert the address to a string. The `addr` parameter can be set to `NULL` if we don't want to know the address and port of the client that is connecting to the server.

- The `addrlen` parameter is the size of the `addr` parameter. This parameter is usually set to `sizeof(struct sockaddr_in)`.

After calling the `accept()` function, we will have a new file descriptor that we can use to send and receive data with the client that is connecting to the server.

### Non-blocking I/O

The next challenge is to make the I/O operations non-blocking. This is important because we want to be able to handle multiple connections at the same time. If we use blocking I/O operations, the program will stop and wait for the data to be sent or received. That means that we will only be able to handle one connection at a time.

We could, theoretically, create a new thread for each connection. However, this is not a good idea. Threads are expensive, and they consume a lot of resources. If we have a lot of connections, we will have a lot of threads, and the program will consume a lot of resources.

The best way to handle multiple connections at the same time is to use non-blocking I/O operations. This means that the program will not stop and wait for the data to be sent or received. Instead, the program will continue to run, and we will be able to handle multiple connections at the same time.

The `accept()` function is a blocking function. That means that the program will stop and wait for a client to connect to the server. 

We can, however, make the `accept()` function non-blocking by using the `fcntl()` function to set the `O_NONBLOCK` flag on the socket. This flag will make the `accept()` function return immediately if there are no pending connections. 


#### The `fcntl()` function

```c++
int fcntl(int fd, int cmd, ... /* arg */ );
```

The `fcntl()` function is used to perform various operations on a file descriptor. It takes three parameters:

- The `fd` parameter is the file descriptor that we want to perform the operation on;

- The `cmd` parameter is the operation that we want to perform. We can use the `F_SETFL` command to set the file status flags. We can use the bitwise OR operator to set the `O_NONBLOCK` flag on the socket.

- The `arg` parameter is the value that we want to set. We can use the `O_NONBLOCK` flag to make the `accept()` function non-blocking.

After calling the `fcntl()` function, the `accept()` function will return immediately if there are no pending connections.

#### The `poll()` function

```c++
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

The `poll()` function is a non-blocking function that is used to monitor file descriptors for I/O events. It takes three parameters:

- The `fds` parameter is a pointer to an array of `pollfd` structures. 
	```c++
	struct pollfd {
		int fd;         /* file descriptor */
		short events;   /* requested events */
		short revents;  /* returned events */
	};
	```
	- The `fd` member is the file descriptor that we want to monitor.
	- The `events` member is the events that we want to monitor. There are macros for different events, such as `POLLIN` for data to be read, `POLLOUT` for data to be written, and `POLLERR` for an error condition. We can use the bitwise OR operator to monitor multiple events at the same time.
	- The `revents` member is the events that occurred. This member is set by the kernel, and we can use the bitwise AND operator to check if a specific event occurred.

- The `nfds` parameter is the number of file descriptors that we want to monitor.

- The `timeout` parameter is the maximum amount of time, in milliseconds, that we want to wait for an event to occur. This parameter is usually set to `-1`, which means that we want to wait indefinitely (making it a blocking function all the same). You can even set it to `0`, which means that we don't want to wait at all.

The `poll()` function returns the number of file descriptors that have events that occurred, `0` if no events occurred before the timeout, or `-1` if an error occurred.

### Multiple sockets = multiple clients

The most important thing to understand about servers is this: In order to accept multiple clients, we need to create multiple sockets. Each one of these sockets will be responsible for handling the connection with a single client.

There's one socket - called the **server socket** - that is responsible for listening for incoming connections. When a client connects to the server, the server socket will accept the connection and create a new socket to communicate with the client. 

These other sockets - called **client sockets** - are responsible for handling the connection with the client. They are created by the server socket, and they are used to send and receive data with the client.

This is important to understand: the server socket is not used to send and receive data with the client. It's only used to accept incoming connections. The client sockets are used to send and receive data with the client.

<h2>
The IRC protocol
</h2>

Now, we know how servers and clients communicate with each other. We know how to create a server in C++, and we know how to handle multiple connections at the same time. This, however, is just the very first step.

In order for our server to be considered an IRC server, it must learn how "to speak" the IRC protocol. This is the core of the project, and where the real challenge lies.

The IRC protocol is a text-based protocol. That means that all communication is done through text messages. These messages are sent and received through the sockets, and they follow a specific set of rules.

The IRC protocol is described in the RFC 1459 and the RFC 2810 to 2813. These documents describe the rules that we must follow in order to create an IRC server. We won't need (God forbid!) to implement the entire protocol. That would be a nightmare. But we must implement the most important parts of it.

We also need to choose a client to use as a reference. For this project, we chose `HexChat`, which is an open-source IRC client that is available for Windows, macOS, and Linux. We are going to use it to connect to our server and to test the server's responses to the client's actions.

#### When a new client connects to the server

When a new client connects to the server, the client sends a series of messages to the server who must, in turn, respond appropriately, first authenticating the client, and then allowing the client to perform a series of actions.

The messages that the client sends to the server are called **commands**. The server must respond to these commands with a series of **replies**. These replies are also messages, and they follow a specific format, for example:

```
:irc.example.com 001 nick :Welcome to the Internet Relay Network nick!user@host
```

This is a reply to the `NICK` command. The server sends this message to the client in order to welcome the client to the server. The message contains the server's address, the reply code, the client's nickname, and a welcome message.

The server must respond to all commands with the appropriate replies. The server must also handle the client's actions, such as joining a channel, sending a message, and performing a series of other actions.

#### The most important commands

The server must be able to handle the following commands:

- **NICK**: to set a nickname;
- **USER**: to set a username;
- **JOIN**: to join a channel;
- **PRIVMSG**: to send a private message;
- **PING**: to keep the connection alive;
- **PONG**: to respond to the `PING` command;
- **QUIT**: to disconnect from the server;

The server must also be able to handle the following commands, regarding the channels:

- **KICK**: to kick a user from a channel;
- **INVITE**: to invite a user to a channel;
- **TOPIC**: to set the topic of a channel;
- **MODE**: to set the mode of a channel. For this command, the server must be able to handle the `i`, `t`, `k`, `o`, and `l` parameters;

The server must also be able to handle the following commands, regarding the users:

- **WHOIS**: to get information about a user;
- **WHO**: to get a list of users;
- **LIST**: to get a list of channels;
- **NAMES**: to get a list of users in a channel;

#### Authentication

The server must be able to authenticate the client. The server is secured by a password, and the client must send the password to the server before being able to perform any action.

To authenticate the client, the server must be able to handle the `PASS` command. The client sends the password to the server, and the server responds with the appropriate reply.

If the client is correctly authenticated, the server must respond with the `001` reply, welcoming the client to the server.

```
:irc.example.com 001 nick :Welcome to the Internet Relay Network nick!user@host
```

If not, you can choose how to handle: if asking for the password again, or if disconnecting the client from the server altogether. For example:

```
:irc.example.com 464 :Password incorrect
```

### After authentication

After the client is properly authenticated, with the user and nickname set, the server must be able to handle all other commands. By default, a IRC client demands the user to either join a channel or to send a private message to another user. 

Other commands are also available, like `LIST`, `NAMES`, `WHO`, and `WHOIS`. The server must be able to handle these commands, and respond with the appropriate replies.
