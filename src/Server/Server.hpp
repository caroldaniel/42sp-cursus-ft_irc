/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:43 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/06 14:09:13 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <cstring>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <stdexcept>

/*
** Server class
*/

class Server {
private:
    // Attributes
    int _port;
    int _socket;
    // Private methods
    bool _server_init(int port);
    int  _accept_connection(void);
public:
    // Constructors
    Server();
    Server(int port);
    // Copy constructor
    Server(const Server &other);
    // Destructor
    ~Server();
    // Assignment operator
    Server &operator=(const Server &other);
    // Getters and setters
    int getPort(void) const;
    void setPort(int port);

    // Member functions
    bool start(void);
};

#endif