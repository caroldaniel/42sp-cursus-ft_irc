/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:38 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/08 12:12:52 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Server::Server(std::string port, std::string password) : _running(false), _socket(-1), _port(port), _password(password), _hostname("localhost") {
    std::cout << "Server created" << std::endl;
    return ;
}

Server::Server(const Server &other) {
    *this = other;
    return ;
}

Server::~Server(void) {
    if (_socket != -1) {
        close(_socket);
    }
    for (size_t i = 0; i < _pollfds.size(); i++) {
        close(_pollfds[i].fd);
    }
    // Delete all clients from the map using c++98 syntax
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        delete it->second;
    }
    std::cout << "Server destroyed" << std::endl;
}

/******************************************************************************/
/*                            Operator Overloads                              */
/******************************************************************************/

Server &Server::operator=(const Server &other) {
   *this = other;
    return *this;
}

/******************************************************************************/
/*                         Private member functions                           */
/******************************************************************************/


void Server::_on_client_connection(void) {
    // Prepare the client address structure    
    sockaddr_in  client_address;
    socklen_t    client_address_size = sizeof(client_address);
    int          client_socket;

    // Accept the client connection
    client_socket = accept(_socket, (struct sockaddr *)&client_address, &client_address_size);
    if (client_socket == -1) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
    addPollfd(_pollfds, client_socket, POLLIN);

    // Get client information
    char hostname[NI_MAXHOST];
    int result = getnameinfo((struct sockaddr *)&client_address, client_address_size, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    if (result != 0) {
        throw std::runtime_error(std::string(gai_strerror(result)));
    }
    
    Client  *client = new Client(client_socket, ntohs(client_address.sin_port), hostname);  
     
    _clients.insert(std::make_pair(client_socket, client));
    std::cout << client->get_hostname() << ":" << client->get_port() << " has connected" << std::endl;
    
    return ;
}

/******************************************************************************/
/*                              Member functions                              */
/******************************************************************************/

void Server::create_socket(void) {
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        throw std::runtime_error("Error creating server socket");
    }
    
    // Make the server socket reusable
    int enable = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
        throw std::runtime_error("Error setting server socket options");
    }

    // Make the server socket non-blocking
    if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1) {
        throw std::runtime_error("Error setting server socket to non-blocking");
    }

    // Prepare the server address structure
    struct sockaddr_in server_address = {};
    bzero((char *)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = INADDR_ANY; 
    server_address.sin_port = htons(std::atoi(_port.c_str()));

    // Bind the server socket to the server address and port
    if (bind(_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        throw std::runtime_error("Error binding server socket to port");
    }
    
    // Listen for connections
    if (listen(_socket, 5) == -1) {
        throw std::runtime_error("Error listening on server socket");
    }
    return ;
}

void Server::start(void) {
    // Set the server as running
    _running = true;
    std::cout << "Server started" << std::endl;
    
    // Add the server socket to the vector
    addPollfd(_pollfds, _socket, POLLIN);

    // Add STDIN to the vector
    addPollfd(_pollfds, STDIN_FILENO, POLLIN);    
    
    // Poll for events
    while (_running) {
        int poll_count = poll(_pollfds.data(), _pollfds.size(), 0);

        // Check for errors in the poll function
        if (poll_count == -1) {
            throw std::runtime_error("Cannot poll for events: " + std::string(strerror(errno)));
        }

        // If no events happened, continue
        if (poll_count == 0) {
            continue;
        }
        
        // Check if the server socket has events
        if (_pollfds[0].revents & POLLIN) {
            try {
                _on_client_connection();
            } catch (std::exception &e) {
                throw std::runtime_error("Cannot accept client connection: " + std::string(strerror(errno)));
            }
        }
        
        // Check if the STDIN has received data and, if so, if the data is "exit" or an EOF
        if (_pollfds[1].revents & POLLIN) {
            // read from STDIN
            char buffer[1024];
            int bytes_received = read(STDIN_FILENO, buffer, sizeof(buffer));
            if (bytes_received == -1) {
                throw std::runtime_error("Error reading from STDIN: " + std::string(strerror(errno)));
            }
            if ((bytes_received == 0) || (std::string(buffer, bytes_received) == "exit\n")) {
                _running = false;
            }
        }
        
        // Loop through the client sockets
        for (size_t i = 2; i < _pollfds.size(); i++) {
            // If the client socket has events, receive data
            if ((_pollfds[i].revents & POLLIN) == POLLIN) {
        
                char buffer[1024];
                int bytes_received = recv(_pollfds[i].fd, buffer, sizeof(buffer), 0);
                
                // Check for errors in the recv function
                if (bytes_received == -1) {
                    throw std::runtime_error("Error receiving data from client: " + std::string(strerror(errno)));
                }

                // If the client socket is closed, remove it from the vector
                if (bytes_received == 0) {
                    close(_pollfds[i].fd);
                    _pollfds.erase(_pollfds.begin() + i);
                    continue;
                }

                // Print the received data
                std::cout << "Received: " << std::string(buffer, bytes_received) << std::endl;
            }
        }
    }
}
