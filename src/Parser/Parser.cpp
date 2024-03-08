/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dofranci <dofranci@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-03-08 21:07:16 by dofranci          #+#    #+#             */
/*   Updated: 2024-03-08 21:07:16 by dofranci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

// bool checkDataExist(int fd, std::map<int, Client *> _clients, std::string *data)
// {

// }

void Parser::setupClientData(int fd, std::map<int, Client *> _clients, std::string message)
{
    std::string data[4];
    std::istringstream iss(message);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.find("NICK") != std::string::npos) {
            //nickname
            data[0] = line.substr(line.find("NICK") + 5);
        } else if (line.find("USER") != std::string::npos) {
            //username
            line.erase(0, 5);
            data[1] = line.substr(0 ,line.find(" "));
            //realname
            data[2] = line.substr(line.find_first_of(":") + 1);
        } else if (line.find("PASS") != std::string::npos) {
            //password
            data[3] = line.substr(line.find("PASS") + 5);
        }
    } // NEED VERIFY LENGTH OF FIELDS

    // if(checkDataExist(fd, _clients, data)) {
        _clients[fd]->set_nickname(data[0]);
        _clients[fd]->set_username(data[1]);
        _clients[fd]->set_realname(data[2]);
        _clients[fd]->set_password(data[3]);
        std::string welcome = _clients[fd]->get_nickname() + ": Welcome to the COOLEST IRC SERVER, " + _clients[fd]->get_nickname() + "[!" + _clients[fd]->get_username() + "@" + _clients[fd]->get_hostname() + "]\r\n";
        send(_clients[fd]->get_socket(), welcome.c_str(), welcome.length(), 0);
    //}
    //else
        // send a message of the failure registration
/*
    // DEBUG PRINT CLIENT DATA
    std::cout << "Client hostname: " << _clients[fd]->get_hostname() << std::endl << \
    "Client nickname: " << _clients[fd]->get_nickname() << std::endl << \
    "Client username: " << _clients[fd]->get_username() << std::endl << \
    "Client realname: " << _clients[fd]->get_realname() << std::endl << \
    "Client password: " << _clients[fd]->get_password() << std::endl;
*/
}

void Parser::handleClientMessage(int fd, std::map<int, Client *> _clients, std::string message)
{
    std::cout << std::endl;
    if(message.find("CAP LS") != std::string::npos)
        Parser::setupClientData(fd, _clients, message);
    else
        std::cout << "Received: " << message << "From: " << _clients[fd]->get_hostname() << std::endl; //treat command
    std::cout << std::endl;
    
}
