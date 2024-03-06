/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:38 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/06 19:20:16 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Server::Server(int port, std::string password) : _port(port), _password(password), _socket(-1) {
    std::cout << "Server created" << std::endl;
    return ;
}

Server::Server(const Server &other) {
    this->_port = other._port;
    this->_password = other._password;
    this->_socket = other._socket;
    std::cout << "Server copied" << std::endl;
    return ;
}

Server::~Server(void) {
    if (this->_socket != -1) {
        close(this->_socket);
    }
    std::cout << "Server destroyed" << std::endl;
}

/******************************************************************************/
/*                            Operator Overloads                              */
/******************************************************************************/

Server &Server::operator=(const Server &other) {
    this->_port = other._port;
    this->_password = other._password;
    this->_socket = other._socket;
    std::cout << "Server assigned" << std::endl;
    return *this;
}

/******************************************************************************/
/*                              Member functions                              */
/******************************************************************************/

void Server::create_socket(void) {
    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == -1) {
        throw std::runtime_error("Error creating server socket");
    }
    
    // Make the server socket reusable
    int enable = 1;
    if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
        throw std::runtime_error("Error setting server socket options");
    }

    // Make the server socket non-blocking
    if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) == -1) {
        throw std::runtime_error("Error setting server socket to non-blocking");
    }

    // Prepare the server address structure
    struct sockaddr_in server_address = {};
    bzero((char *)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET; // Address family for IPv4
    server_address.sin_addr.s_addr = INADDR_ANY; // Listen on any IP address
    server_address.sin_port = htons(this->_port); // Listen on the specified port

    // Bind the server socket to the server address and port
    if (bind(this->_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        throw std::runtime_error("Error binding server socket to port");
    }
    
    // Listen for connections
    if (listen(this->_socket, 5) == -1) {
        throw std::runtime_error("Error listening on server socket");
    }

    return ;
}

void Server::start(void) {

    int client_socket = this->_accept_connection();
    if (client_socket == -1) {
        throw std::runtime_error("Error accepting client connection");
    }
    std::cout << "Connection accepted" << std::endl;

    // Print the message from the client
    char buffer[256];
    int bytes_received = recv(client_socket, buffer, 256, 0);
    if (bytes_received == -1) {
        std::cerr << "Error receiving message from client" << std::endl;
    }
    std::cout << "Message received: " << buffer << std::endl;

    // Close the client socket
    close(client_socket);
    return ;
}

/******************************************************************************/
/*                         Private member functions                           */
/******************************************************************************/


int Server::_accept_connection(void) {
    // Prepare the client address structure    
    sockaddr_in  client_address;
    socklen_t    client_address_size = sizeof(client_address);
    int          client_socket;

    // Accept the client connection
    client_socket = accept(this->_socket, (struct sockaddr *)&client_address, &client_address_size);
    
    return client_socket;
}
