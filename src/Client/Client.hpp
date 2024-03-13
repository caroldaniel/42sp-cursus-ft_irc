/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 14:25:21 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 21:18:03 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <cstdlib>
# include <cstring>
# include <string>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <stdexcept>
# include <cerrno>
# include <poll.h>
# include <algorithm>
# include <vector>
# include <map>
# include <sstream>

# include "../utils/utils.hpp"
# include "../utils/IRCmacros.hpp"

class Channel;

class Client
{
private:

    // Attributes
    std::string             _server_hostname;
    int                     _socket;
    int                     _port;
    const std::string       _password;
    
    bool                    _disconnected;
    bool                    _authenticated;

    std::string             _nickname;
    std::string             _username;
    std::string             _realname;
    std::string             _hostname;

public:
    // Constructors
    Client(std::string server_hostname, int fd, int port, std::string password, const std::string &hostname);
    Client(const Client &other);
    // Destructor
    ~Client();
    // Assignment operator
    Client &operator=(const Client &other);

    // Member functions
    void        disconnect(void);
    void        authenticate(std::string password);
    void        reply(std::string  code, std::string command, std::string message);
    void        broadcast(Client *sender, std::string target, std::string message);

    // Getters
    std::string get_server_hostname(void) const;
    int         get_socket(void) const;
    int         get_port(void) const;
    std::string get_nickname(void) const;
    std::string get_username(void) const;
    std::string get_realname(void) const;
    std::string get_hostname(void) const;

    bool        is_disconnected(void) const;
    bool        is_authenticated(void) const;
    bool        is_registered(void) const;

    // Setters
    void        set_nickname(const std::string &nickname);
    void        set_username(const std::string &username);
    void        set_realname(const std::string &realname);
    
};


#endif