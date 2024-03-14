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
    client->set_nickname(message->get_params()[0]);
    if (client->is_authenticated() && client->is_registered()) {
        client->reply(RPL_WELCOME, "", ":Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());
    }    
    return ;
}
