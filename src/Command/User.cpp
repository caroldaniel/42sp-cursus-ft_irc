/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:36:12 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 14:43:54 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

User::User(Server *server) : Command("USER", server) {
    return ;
}

User::~User(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    User::invoke(Client *client, Message *message) {
    // Check if message has enough parameters
    if (message->get_params().size() < 4) {
        client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
        return ;
    }
    // Check if the user is already registered
    if (client->is_registered()) {
        client->reply(ERR_ALREADYREGISTRED, _name, ":You may not reregister");
        return ;
    }
    client->set_username(message->get_params()[0]);
    client->set_realname(message->get_params()[3]);
    if (client->is_authenticated() && client->is_registered()) {
        client->reply(RPL_WELCOME, "", ":Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());
    }    
    return ;
}