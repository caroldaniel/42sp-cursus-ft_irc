/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:43 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/08 12:07:21 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

/*
** Libraries
*/
# include "../utils/utils.hpp"
# include "../Client/Client.hpp"

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

    // Private methods
    void         _on_client_connection(void);

public:

    // Constructors
    Server(std::string port, std::string password);
    // Copy constructor
    Server(const Server &other);
    // Destructor
    ~Server();
    // Assignment operator
    Server &operator=(const Server &other);

    // Member functions
    void create_socket(void);
    void start(void);
};

#endif