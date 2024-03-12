/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:43 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/11 21:09:58 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

/*
** Libraries
*/
# include "../utils/utils.hpp"
# include "../Client/Client.hpp"
# include "../Message/Message.hpp"
# include "../Command/Command.hpp"

/*
** Server class
*/

class Server {
private:

    // Attributes
    bool                    _running;
    int                     _socket;
    
    const std::string       _port;
    const std::string       _password;
    const std::string       _hostname;

    std::vector<pollfd>     _pollfds;
    std::map<int, Client *> _clients;

    std::map<std::string, Command *> _commands;

public:

    // Constructors
    Server(std::string port, std::string password);
    Server(const Server &other);
    // Destructor
    ~Server();
    // Assignment operator
    Server  &operator=(const Server &other);

    // Getters
    Client  *get_client(int client_fd);

    // Member functions on Client's actions
    void    on_client_connect(void);
    void    on_client_disconnect(int client_fd);
    void    on_client_message(int client_fd, std::string message);

    // Member functions on Server's actions
    void    create_socket(void);
    void    start(void);

    // Invoke a command
    void    invoke_command(Client *client, Message *message);
};

#endif