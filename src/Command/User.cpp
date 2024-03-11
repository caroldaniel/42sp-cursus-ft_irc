/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:36:12 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/10 23:23:03 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

User::User(Server *server, std::vector<std::string> tokens) : Command(server, tokens) {
    return ;
}

User::~User(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    User::execute_command(Client *client) {
    std::string username = _tokens[1];
    std::string realname = _tokens[4].substr(1);    
    
    client->set_username(username);
    client->set_realname(realname);
    client->send_reply("001", "Welcome to the Internet Relay Network " + username + "!" + client->get_hostname());
    return ;
}