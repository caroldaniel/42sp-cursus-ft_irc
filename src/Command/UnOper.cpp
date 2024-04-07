/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnOper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 22:36:11 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/06 21:02:41 by cado-car         ###   ########.fr       */
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
            client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
            return ;
        }

        // Check if the client has the necessary permissions
        if (!client->is_oper()) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, _name, ":Permission Denied");
            return ;
        }

        std::string target_name = message->get_params()[0];
        Client  *target = _server->get_client_by_nickname(target_name);
        if(target == NULL)
            client->reply(ERR_NOSUCHNICK, target_name, ":No such nick/channel");
        else {
            if (!target->is_oper()) {
                client->reply(ERR_NOTOPER, target_name, ":is not an IRC operator");
                return ;
            }
            target->unOper();
            if (client->get_nickname() != target_name)
                client->reply(ERR_NOTOPER, target_name, ":Is no longer an IRC operator");
            target->reply(ERR_NOTOPER, client->get_nickname(), ":You are no longer an IRC operator");
        }
    }
}