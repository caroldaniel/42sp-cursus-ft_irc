/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:38 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/10 23:15:54 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Server::Server(std::string port, std::string password) : _running(false), _socket(-1), _port(port), _password(password), _hostname("127.0.0.1") {
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
    // Delete all clients from the map using c++98 syntax
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        delete it->second;
    }
    for (size_t i = 0; i < _pollfds.size(); i++) {
        close(_pollfds[i].fd);
    }
}

/******************************************************************************/
/*                            Operator Overloads                              */
/******************************************************************************/

Server &Server::operator=(const Server &other) {
   *this = other;
    return *this;
}

/******************************************************************************/
/*                                 Getters                                    */
/******************************************************************************/

Client  *Server::get_client(int client_fd) {
    std::map<int, Client *>::iterator it = _clients.find(client_fd);
    if (it == _clients.end()) {
        throw std::runtime_error("Client not found");
    }
    return it->second;
}

/******************************************************************************/
/*                  Member functions on Client's actions                      */
/******************************************************************************/


void Server::on_client_connect(void) {
    // Prepare the client address structure    
    sockaddr_in  client_address;
    socklen_t    client_address_size = sizeof(client_address);
    int          client_socket;

    // Accept the client connection
    client_socket = accept(_socket, (struct sockaddr *)&client_address, &client_address_size);
    if (client_socket == -1) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
    addPollfd(_pollfds, client_socket, POLLIN | POLLHUP);

    // Get client information
    char hostname[NI_MAXHOST];
    int result = getnameinfo((struct sockaddr *)&client_address, client_address_size, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    if (result != 0) {
        throw std::runtime_error(std::string(gai_strerror(result)));
    }
    
    Client  *client = new Client(_hostname, client_socket, ntohs(client_address.sin_port), _password, hostname);  
     
    _clients.insert(std::make_pair(client_socket, client));
    std::cout << client->get_hostname() << ":" << client->get_port() << " has connected" << std::endl;
    
    return ;
}

void Server::on_client_disconnect(int client_fd) {
    // Remove the client from the map
    std::map<int, Client *>::iterator it = _clients.find(client_fd);
    if (it != _clients.end()) {
        it->second->disconnect();
        delete it->second;
        _clients.erase(it);
    }
    // Remove the client from the vector
    for (size_t i = 0; i < _pollfds.size(); i++) {
        if (_pollfds[i].fd == client_fd) {
            _pollfds.erase(_pollfds.begin() + i);
            break;
        }
    }
    return ;
}

void Server::on_client_message(int client_fd, std::string message) {
    // Parse line by line
    std::istringstream iss(message); 
    std::string line;
    while (std::getline(iss, line)) {
        
    }
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

    // Create the server socket
    try {
        create_socket();
    } catch (std::exception &e) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
     
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
                on_client_connect();
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
                throw std::runtime_error("Cannot read from STDIN: " + std::string(strerror(errno)));
            }
            if ((bytes_received == 0) || (std::string(buffer, bytes_received) == "exit\n")) {
                _running = false;
            }
        }
        
        for (size_t i = 2; i < _pollfds.size(); i++) {
            
            // If the client socket disconnected, remove it
            if ((_pollfds[i].revents & POLLHUP) == POLLHUP) {
                on_client_disconnect(_pollfds[i].fd);
                continue;
            }
            
            // If the client socket has events, receive data
            if ((_pollfds[i].revents & POLLIN) == POLLIN) {
        
                char buffer[1024];
                int bytes_received = recv(_pollfds[i].fd, buffer, sizeof(buffer), 0);
                
                // Check for errors in the recv function
                if (bytes_received == -1) {
                    throw std::runtime_error("Cannot receive data from client: " + std::string(strerror(errno)));
                }

                // If the client socket is closed, remove it from the vector
                if (bytes_received == 0) {
                    on_client_disconnect(_pollfds[i].fd);
                    continue;
                }

                // Print the received data
                on_client_message(_pollfds[i].fd, std::string(buffer, bytes_received));
            }
        }
    }
}

void Server::authenticate_client(int client_fd, std::string password) {
    std::map<int, Client *>::iterator it = _clients.find(client_fd);
    if (it != _clients.end()) {
        try {
            it->second->authenticate(password);
        } catch (std::exception &e) {
            throw std::runtime_error("Cannot authenticate client: " + std::string(e.what()));
        }
    }
    return ;
}
