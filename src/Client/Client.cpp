/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 14:25:23 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/25 21:57:25 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Client::Client(std::string server_hostname, int fd, int port, std::string password, const std::string &hostname) 
    : _server_hostname(server_hostname), 
    _socket(fd), 
    _port(port), 
    _password(password),
    _disconnected(false), 
    _authenticated(false),
    _registered(false),
    _oper(false), 
    _nickname(""), 
    _username(""), 
    _realname(""), 
    _hostname(hostname) {
    std::cout << "Client on " << _hostname << ":" << _port << " created" << std::endl;
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

void        Client::disconnect(std::string message) {
    
    if (_disconnected == false) {
        reply(RPL_QUIT, message);
        close(_socket);
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

void        Client::oper(std::string oper_password) {
    // compare the password
    if (oper_password == g_oper_password) {
        _oper = true;
    }
    return ;
}

void        Client::unOper(void) {
    _oper = false;
    return ;
}

void        Client::reply(std::string code, std::string message) {
    // Concatenate the message and send it to the client
    std::string code_str;
    std::string nickname_str;
    std::string command_str;

    code_str = code.empty() ? "" : code + SPACE;
    nickname_str = _nickname.empty() ? "unregistered " : _nickname + SPACE;
    
    std::string reply;
    reply = ":" + _server_hostname + SPACE + code_str + nickname_str + message + CRLF;

    std::cout << "Sending reply to " << _nickname << "@" << _hostname << ":" << _port << ": " << reply << std::endl;

    send(_socket, reply.c_str(), reply.length(), 0);
    return ;
}

void        Client::broadcast(Client *sender, std::string target, std::string message) {
    // Concatenate the message and send it to the client
    std::string reply = ":" + sender->get_nickname() + SPACE + "PRIVMSG" + SPACE + target + ":" + _server_hostname + SPACE + ":" + message + CRLF;
    std::cout << "Broadcasting message to " << target << ": " << reply << std::endl;
    send(_socket, reply.c_str(), reply.length(), 0);
    return ;
}

/******************************************************************************/
/*                                 Getters                                    */
/******************************************************************************/

std::string Client::get_server_hostname(void) const {
    return _server_hostname;
}

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

bool        Client::is_oper(void) const {
    return _oper;
}

bool        Client::is_registered(void) const {
    return _registered;
}

bool        Client::has_nickname(void) const {
    return !_nickname.empty();
}

/******************************************************************************/
/*                                 Setters                                    */
/******************************************************************************/

void        Client::set_nickname(const std::string &nickname) {
    reply(RPL_NEWNICK, ":Nickname set to <" + nickname + ">");
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

void        Client::set_registered(bool registered) {
    _registered = registered;
    return ;
}
