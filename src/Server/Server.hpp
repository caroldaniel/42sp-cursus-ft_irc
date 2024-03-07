/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:43 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/06 22:07:28 by user42           ###   ########.fr       */
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
# include <poll.h>
# include <vector>

/*
** Libraries
*/
# include "../utils/utils.hpp"

/*
** Server class
*/

class Server {
private:

    // Attributes
    int         _port;
    std::string _password;
    int         _socket;
    bool        _running;

    // Private methods
    int         _accept_connection(void);

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