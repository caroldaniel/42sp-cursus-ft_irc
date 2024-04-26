/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 17:47:54 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 18:25:37 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

Notice::Notice(Server *server) : Command("NOTICE", server) {
    return ;
}

Notice::~Notice(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Notice::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        if (message->get_params().size() < 2) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for NOTICE command");
            return;
        }
        
        std::string target_name = message->get_params()[0];
        std::string msg = ":" + message->get_params()[1];
        // Find target client
        Client *target = _server->get_client(target_name);

        // Find PING in msg and replace it by PONG
        size_t found = msg.find("PING");
        if (found != std::string::npos) {
            msg.replace(found, 4, "PONG");
            target->broadcast(client, "PRIVMSG", target_name, msg);
        }
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
