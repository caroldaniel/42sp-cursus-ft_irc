/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 23:09:21 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/25 15:42:54 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Nick::Nick(Server *server) : Command("NICK", server) {
    return ;
}

Nick::~Nick(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Nick::invoke(Client *client, Message *message) {
    
    // Check if message has enough parameters
    if (message->get_params().size() < 1) {
        client->reply(ERR_ERRONEUSNICKNAME, "<empty_nickname>");
        return ;
    }

    std::string new_nick = message->get_params()[0];

    // Check if nickname is empty
    if (new_nick.empty()) {
        client->reply(ERR_NONICKNAMEGIVEN, "<empty_nickname>");
        return ;
    }

    // Check if nickname is already in use   
    if (_server->get_client_by_nickname(new_nick)) {
        client->reply(ERR_NICKNAMEINUSE, "<" + new_nick + ">");
        return;
    }
    
    // Check if nickname is valid
    for (size_t i = 0; i < new_nick.length(); i++) {
        if(!std::isalnum(new_nick[i]) && new_nick[i] != '_')
        {
            client->reply(ERR_ERRONEUSNICKNAME, ":Nickname can only contain letters, numbers and underscores");
            return ;
        }
    }

    // Set new nickname
    client->set_nickname(new_nick);
    if (!client->get_username().empty() && !client->get_realname().empty()) {
        client->set_registered(true);
        client->reply(RPL_WELCOME, ":Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());        
    }
    return ;
}
