/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 14:25:23 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 16:04:48 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Client::Client(std::string server_hostname, int fd, int port, std::string password, const std::string &hostname) 
: _server_hostname(server_hostname), _socket(fd), _port(port), _password(password), _disconnected(false), _authenticated(false), _username(""), _realname(""), _hostname(hostname) {
    std::cout << "Client created" << std::endl;
    return ;
}

Client::Client(const Client &other) {
    *this = other;
    return ;
}

Client::~Client(void) {
    if (_socket != -1) {
        close(_socket);
    }
    std::cout << "Client destroyed" << std::endl;
    return ;
}

/******************************************************************************/
/*                            Operator Overloads                              */
/******************************************************************************/

Client &Client::operator=(const Client &other) {
    *this = other;
    return *this;
}

/******************************************************************************/
/*                             Member functions                               */
/******************************************************************************/

void        Client::disconnect(void) {
    if (_disconnected == false) {
        _disconnected = true;
    }
    return ;
}

void        Client::authenticate(std::string password) {
    // compare the password
    if (password != _password) {
        _authenticated = false;
        return ;
    }
    _authenticated = true;
    return ;
}

void        Client::reply(std::string code, std::string command, std::string message) {
    // Concatenate the message and send it to the client
    std::string reply;
    
    if (command.empty()) {
        reply = ":" + _server_hostname + SPACE + code + SPACE + _nickname + SPACE + message + CRLF;
    } else {
        reply = ":" + _server_hostname + SPACE + code + SPACE + _nickname + SPACE + command + SPACE + message + CRLF;
    }
    std::cout << "Sending reply: " << reply << std::endl;
    send(_socket, reply.c_str(), reply.length(), 0);
    return ;
}

void        Client::broadcast(Client *sender, std::string target, std::string message) {
    // Concatenate the message and send it to the client
    std::string reply = ":" + sender->get_nickname() + "!" + sender->get_username() + "@" + sender->get_hostname() + SPACE + "PRIVMSG" + SPACE + target + SPACE + ":" + message + CRLF;
    std::cout << "Broadcasting: " << reply << std::endl;
    send(_socket, reply.c_str(), reply.length(), 0);
    return ;
}

/******************************************************************************/
/*                                 Getters                                    */
/******************************************************************************/

int         Client::get_socket(void) const {
    return _socket;
}

int         Client::get_port(void) const {
    return _port;
}

std::string Client::get_nickname(void) const {
    return _nickname;
}

std::string Client::get_username(void) const {
    return _username;
}

std::string Client::get_realname(void) const {
    return _realname;
}

std::string Client::get_hostname(void) const {
    return _hostname;
}

bool        Client::is_disconnected(void) const {
    return _disconnected;
}

bool        Client::is_authenticated(void) const {
    return _authenticated;
}
bool        Client::is_registered(void) const {
    return !_nickname.empty() && !_username.empty() && !_realname.empty();
}

/******************************************************************************/
/*                                 Setters                                    */
/******************************************************************************/

void        Client::set_nickname(const std::string &nickname) {
    _nickname = nickname;
    return ;
}

void        Client::set_username(const std::string &username) {
    _username = username;
    return ;
}

void        Client::set_realname(const std::string &realname) {
    _realname = realname;
    return ;
}
