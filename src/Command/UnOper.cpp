/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnOper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dofranci <dofranci@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-03-20 22:36:11 by dofranci          #+#    #+#             */
/*   Updated: 2024-03-20 22:36:11 by dofranci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

UnOper::UnOper(Server *server) : Command("UNOPER", server) {
    return ;
}

UnOper::~UnOper(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void UnOper::invoke(Client *client, Message *message) {
   if (client->is_authenticated() && client->is_registered()) {
        // Check if message has enough parameters
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, _name, ": Not enough parameters");
            return ;
        }

        // Check if the client has the necessary permissions
        if (!client->is_oper()) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, _name, ": Permission Denied");
            return ;
        }

        Client  *target = _server->get_client_by_nickname(message->get_params()[0]);
        if(target == NULL) {
            client->reply(ERR_NOSUCHNICK, message->get_params()[0], ": No such nick/channel");
            return ;
        }
        else {
            target->unOper();
            client->reply("", "", target->get_nickname() + ": You are no longer an IRC operator");
        }
    }
}