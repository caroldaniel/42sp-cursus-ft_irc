/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 17:47:54 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 18:27:29 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

Ping::Ping(Server *server) : Command("PING", server) {
    return ;
}

Ping::~Ping(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Ping::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for PING command");
            return;
        }
        
        std::string msg = message->get_params()[0];
        std::string response = ":" + msg;
        
        client->reply(RPL_PING, response);
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
