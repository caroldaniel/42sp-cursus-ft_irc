/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 14:25:21 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/08 11:42:58 by cado-car         ###   ########.fr       */
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
# include <vector>
# include <map>


class Client
{
private:

    // Attributes
    int         _socket;
    int         _port;

    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
    
public:
    Client(int fd, int port, const std::string &hostname);
    Client(const Client &other);
    ~Client();
    Client &operator=(const Client &other);

    // Getters and Setters
    int         get_socket(void) const;
    int         get_port(void) const;
    std::string get_nickname(void) const;
    std::string get_username(void) const;
    std::string get_realname(void) const;
    std::string get_hostname(void) const;
};


#endif