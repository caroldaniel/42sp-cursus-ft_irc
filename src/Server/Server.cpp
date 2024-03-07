/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:38 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/06 22:46:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Server::Server(int port, std::string password) : _port(port), _password(password), _socket(-1), _running(false) {
    std::cout << "Server created" << std::endl;
    return ;
}

Server::Server(const Server &other) {
    this->_port = other._port;
    this->_password = other._password;
    this->_socket = other._socket;
    this->_running = other._running;
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
    this->_running = other._running;
    std::cout << "Server assigned" << std::endl;
    return *this;
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
    // Set the server as running
    this->_running = true;
    std::cout << "Server started" << std::endl;

    // Create vector of pollfd structures
    std::vector<pollfd> fds;

    // Add the server socket to the vector
    addPollfd(fds, this->_socket, POLLIN);

    // Add STDIN to the vector
    addPollfd(fds, STDIN_FILENO, POLLIN);    
    
    // Poll for events
    while (_running) {
        int poll_count = poll(fds.data(), fds.size(), 0);

        // Check for errors in the poll function
        if (poll_count == -1) {
            throw std::runtime_error("Cannot poll for events: " + std::string(strerror(errno)));
        }

        // If no events happened, continue
        if (poll_count == 0) {
            continue;
        }
        
        // Check if the server socket has events
        if (fds[0].revents & POLLIN) {
            int client_socket = this->_accept_connection();
            if (client_socket == -1) {
                throw std::runtime_error("Cannot accept client connection: " + std::string(strerror(errno)));
            }
            addPollfd(fds, client_socket, POLLIN);
        }
        
        // Check if STDIN has "exit" entered (someone typed "exit" and pressed enter)
        if (fds[1].revents & POLLIN) {
            // read from STDIN
            char buffer[1024];
            int bytes_received = read(STDIN_FILENO, buffer, sizeof(buffer));
            if (bytes_received == -1) {
                throw std::runtime_error("Error reading from STDIN: " + std::string(strerror(errno)));
            }
            if (std::string(buffer, bytes_received) == "exit\n") {
                _running = false;
            }
        }
        
        // Loop through the client sockets
        for (size_t i = 2; i < fds.size(); i++) {
            // If the client socket has events, receive data
            if ((fds[i].revents & POLLIN) == POLLIN) {
        
                char buffer[1024];
                int bytes_received = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                
                // Check for errors in the recv function
                if (bytes_received == -1) {
                    throw std::runtime_error("Error receiving data from client: " + std::string(strerror(errno)));
                }

                // If the client socket is closed, remove it from the vector
                if (bytes_received == 0) {
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    continue;
                }

                // Print the received data
                std::cout << "Received: " << std::string(buffer, bytes_received) << std::endl;
            }
        }
    }

    // Close all client sockets
    for (size_t i = 1; i < fds.size(); i++) {
        close(fds[i].fd);
    }
}
