/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 23:09:21 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 14:43:12 by cado-car         ###   ########.fr       */
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
        client->reply(ERR_NONICKNAMEGIVEN, _name, ":No nickname given");
        return ;
    }

    std::string nick = message->get_params()[0];
    if(nick.empty()){
        client->reply(ERR_NONICKNAMEGIVEN, "", client->get_hostname() + ":No nickname given");
        return ;
    }

    for (size_t i = 0; i < nick.length(); i++) {
        if(!std::isalpha(nick[i]))
        {
            client->reply(ERR_ERRONEUSNICKNAME, "", client->get_hostname() + ":Erroneus nickname, non alphabetic characters are forbidden");
            return ;
        }
    }

    if(_server->get_client_by_nickname(nick) == NULL) {
        client->set_nickname(nick);
        if (client->is_authenticated() && client->is_registered()) {
            client->reply(RPL_WELCOME, "", ":Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());
        }    
    }
    else {
        client->reply(ERR_NICKNAMEINUSE, "", client->get_hostname() + ":Nickname " + nick +" is already in use");
        return;
    }

    return ;
}
