/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:38 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/07 18:53:45 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Server::Server(std::string port, std::string password) : _running(false), _socket(-1), _port(port), _password(password), _hostname("127.0.0.1") {
    _commands["USER"] = new User(this);
    _commands["PASS"] = new Pass(this);
    _commands["NICK"] = new Nick(this);
    _commands["QUIT"] = new Quit(this);
    _commands["LIST"] = new List(this);
    _commands["JOIN"] = new Join(this);
    _commands["PRIVMSG"] = new Privmsg(this);
    _commands["OPER"] = new Oper(this);
    _commands["TOPIC"] = new Topic(this);
    _commands["MODE"] = new Mode(this);
    _commands["PART"] = new Part(this);
    _commands["UNOPER"] = new UnOper(this);
    _commands["KICK"] = new Kick(this);
    _commands["INVITE"] = new Invite(this);
    _commands["WHO"] = new Who(this);
    _commands["CAP"] = new Cap(this);
    return ;
}

Server::Server(const Server &other) {
    *this = other;
    return ;
}

Server::~Server(void) {
    // Close the server socket
    if (_socket != -1) {
        close(_socket);
    }
    // Close all client sockets
    for (size_t i = 2; i < _pollfds.size(); i++) {
        if (_pollfds[i].fd != -1)
            close(_pollfds[i].fd);
    }

    // Delete all clients from the map using c++98 syntax
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        delete it->second;
    }

    // Delete all channels from the vector
    for (size_t i = 0; i < _channels.size(); i++) {
        delete _channels[i];
    }
    
    // Delete commands
    for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++) {
        delete it->second;
    }
    return ;
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

Client  *Server::get_client_by_nickname(std::string nickname) {
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        if (it->second->get_nickname() == nickname) {
            return it->second;
        }
    }
    return NULL;
}

Channel *Server::get_channel(std::string name) {
    for (size_t i = 0; i < _channels.size(); i++) {
        if (_channels[i]->get_name() == name) {
            return _channels[i];
        }
    }
    return NULL;
}

std::string Server::get_hostname(void) {
    return _hostname;
}

std::vector<Channel *> Server::list_channels(void) {
    return _channels;
}

std::vector<Client *> Server::list_clients(void) {
    std::vector<Client *> clients;
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        clients.push_back(it->second);
    }
    return clients;
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
    client_fd = client_fd;
    // Parse line by line
    std::istringstream iss(message); 
    std::string line;
    Message *msg;

    while (std::getline(iss, line)) {
        std::cout << "Received from " << _clients[client_fd]->get_nickname() << ": " << line << std::endl;
        try {
            msg = new Message(line);
            if (_commands.find(msg->get_command()) == _commands.end())
                get_client(client_fd)->reply(ERR_UNKNOWNCOMMAND, msg->get_command(), ":Unknown command");
            else
                _commands[msg->get_command()]->invoke(get_client(client_fd), msg);
            delete msg;
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
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

void Server::add_channel(Channel *channel) {
    _channels.push_back(channel);
    return ;
}

// Global flag to indicate if Ctrl+C was pressed
bool g_ctrlcPressed = false;

// Signal handler function for Ctrl+C
static void handleCtrlC(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "Ctrl+C pressed. Stopping server..." << std::endl;
        g_ctrlcPressed = true;
    }
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
    
     // Set up signal handler for Ctrl+C
    struct sigaction sa;
    sa.sa_handler = handleCtrlC;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) != 0)
        throw std::runtime_error("Error setting up signal handler");

    // Poll for events
    while (_running && !g_ctrlcPressed) {
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

            // If the client received a HUP event, disconnect it
            if ((_pollfds[i].revents & POLLHUP) == POLLHUP) {
                _clients.find(_pollfds[i].fd)->second->disconnect();
            }
            
            // If client has been disconnected, call on_client_disconnect
            if (_clients.find(_pollfds[i].fd)->second->is_disconnected()) {
                on_client_disconnect(_pollfds[i].fd);
                continue ;
            }
          
            // If the client socket has events, receive data
            if ((_pollfds[i].revents & POLLIN) == POLLIN) {
        
                char buffer[1024];
                int bytes_received = recv(_pollfds[i].fd, buffer, sizeof(buffer), 0);
                
                // Check for errors in the recv function
                if (bytes_received == -1) {
                    throw std::runtime_error("Cannot receive data from client: " + std::string(strerror(errno)));
                }

                // Print the received data
                on_client_message(_pollfds[i].fd, std::string(buffer, bytes_received));
            }
        }
    }
}
