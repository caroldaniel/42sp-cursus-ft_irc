/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 14:25:23 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/08 12:02:16 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Client::Client(int fd, int port, const std::string &hostname) : _socket(fd), _port(port), _autent(0), _nickname(""), _username(""), _realname(""), _hostname(hostname) {
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
/*                         Getters and Setters                                */
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

std::string Client::get_password(void) const {
    return _password;
}

bool Client::get_autent(void) const {
    return _autent;
}

void Client::set_nickname(const std::string nickname) {
    this->_nickname = nickname;
}

void Client::set_username(const std::string username) {
    this->_username = username;
}

void Client::set_realname(const std::string realname) {
    this->_realname = realname;
}

void Client::set_password(const std::string password) {
    this->_password = password;
}

void Client::set_autent(const bool autent) {
    this->_autent = autent;
}

