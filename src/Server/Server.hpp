/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:43 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/06 19:14:32 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdexcept>
# include <cerrno>

/*
** Server class
*/

class Server {
private:
    // Attributes
    int _port;
    std::string _password;
    int _socket;
    // Private methods
    int  _accept_connection(void);
public:
    // Constructors
    Server(int port, std::string password);
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