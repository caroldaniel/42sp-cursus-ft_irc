/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:38 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/06 14:09:22 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Server::Server(void) : _port(4242), _socket(-1) {
    if (!this->_server_init(this->_port)) {
        throw std::runtime_error("Error initializing server");
    }
    return ;
}

Server::Server(int port) : _port(port), _socket(-1) {
    if (!this->_server_init(port)) {
        throw std::runtime_error("Error initializing server");
    }
    return ;
}

Server::Server(const Server &other) {
    this->_port = other._port;
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
    this->_socket = other._socket;
    std::cout << "Server assigned" << std::endl;
    return *this;
}

/******************************************************************************/
/*                              Member functions                              */
/******************************************************************************/

int Server::getPort(void) const {
    return this->_port;
}

void Server::setPort(int port) {
    this->_port = port;
    std::cout << "Server port set to " << port << std::endl;
}

/******************************************************************************/
/*                         Private member functions                           */
/******************************************************************************/

bool Server::_server_init(int port) {
    // Create the server socket
    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == -1) {
        std::cerr << "Error initializing server socket" << std::endl;
        return false;
    }
    // Bind the server socket to the server port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(this->_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error binding server socket to port " << port << std::endl;
        return false;
    }
    // Listen for connections
    if (listen(this->_socket, 5) == -1) {
        std::cerr << "Error listening on server socket" << std::endl;
        return false;
    }    
    std::cout << "Server initialized and listening to port " << port << std::endl;
    return true;
}

int Server::_accept_connection(void) {
    struct sockaddr_in client_address;

    socklen_t client_address_size = sizeof(client_address);
    int client_socket = accept(this->_socket, (struct sockaddr *)&client_address, &client_address_size);
    return client_socket;
}

bool Server::start(void) {
    int client_socket = this->_accept_connection();

    if (client_socket == -1) {
        std::cerr << "Error accepting connection" << std::endl;
        return false;
    }
    std::cout << "Connection accepted" << std::endl;
    // Print the message from the client
    char buffer[256];
    int bytes_received = recv(client_socket, buffer, 256, 0);
    if (bytes_received == -1) {
        std::cerr << "Error receiving message from client" << std::endl;
    }
    std::cout << "Message received: " << buffer << std::endl;
    return true;
}