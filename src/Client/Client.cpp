/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 14:25:23 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/27 18:53:39 by cado-car         ###   ########.fr       */
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
    this->_server_hostname = other._server_hostname;
    this->_socket = other._socket;
    this->_port = other._port;
    this->_password = other._password;
    this->_disconnected = other._disconnected;
    this->_authenticated = other._authenticated;
    this->_registered = other._registered;
    this->_oper = other._oper;
    this->_nickname = other._nickname;
    this->_username = other._username;
    this->_realname = other._realname;
    this->_hostname = other._hostname;
    return *this;
}

/******************************************************************************/
/*                   Member functions: Client management                      */
/******************************************************************************/

void        Client::disconnect(std::string message) {
    if (!_disconnected) {
        reply(RPL_QUIT, message);
        close(_socket);
        _disconnected = true;
    }
    return ;
}

void        Client::authenticate(std::string password) {
    if (password == _password)
        _authenticated = true;
    return ;
}

void        Client::register_client(void) {
    if (!this->has_nickname())
        reply(ERR_NONICKNAMEGIVEN, ":You must choose a nickname before registering");
    else if (!this->get_username().empty() && !this->get_realname().empty()) {
        _registered = true;
        this->reply(RPL_WELCOME, ":Welcome to the Internet Relay Network " + this->get_nickname() + "!" + this->get_username() + "@" + this->get_hostname());        
    }
    return ;
}

void        Client::oper(std::string oper_password) {
    if (oper_password == g_oper_password)
        _oper = true;
    return ;
}

void        Client::unOper(void) {
    _oper = false;
    return ;
}

/******************************************************************************/
/*                    Member functions: Message handling                      */
/******************************************************************************/

void        Client::reply(std::string code, std::string message) {
    // Server->Client reply
    std::string hostname_str;
    std::string code_str;
    std::string nickname_str;

    hostname_str = ":" + _server_hostname + SPACE;
    code_str = code.empty() ? "" : code + SPACE;
    nickname_str = _nickname.empty() ? "unregistered " : _nickname + SPACE;
    
    // Format ":<server_hostname> <code> <nickname> :<message>\r\n"
    std::string reply;
    reply = hostname_str + code_str + nickname_str + message + CRLF;

    // Server log
    if (g_logs)
        std::cout << "Reply: " << reply << std::endl;

    // Send reply to client
    send(_socket, reply.c_str(), reply.length(), 0);
    return ;
}

void        Client::broadcast(Client *sender, std::string command, std::string target, std::string message) {
    // Client->Client or Client->Channel broadcast
    std::string sender_str;
    std::string command_str;
    std::string target_str;
    std::string message_str;
    
    sender_str = ":" + sender->get_nickname() + "!" + sender->get_username() + "@" + sender->get_hostname() + SPACE;
    command_str = command + SPACE;
    target_str = target + SPACE;
    message_str = command == "KICK" || command == "INVITE" || message.empty() || message[0] == ':' ? message : ":" + message;
    
    // Format ":<sender> <command> <target> :<message>\r\n"
    std::string reply = sender_str + command_str + target_str + message_str + CRLF;

    // Server log
    if (g_logs)
        std::cout << "Broadcast: " << reply << std::endl;

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

bool        Client::is_registered(void) const {
    return _registered;
}

bool        Client::is_oper(void) const {
    return _oper;
}

bool        Client::has_nickname(void) const {
    return !_nickname.empty();
}

/******************************************************************************/
/*                                 Setters                                    */
/******************************************************************************/

void        Client::set_nickname(const std::string &nickname) {
    reply(RPL_NEWNICK, ":Nickname set to " + nickname);
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
