/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnOper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 22:36:11 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/26 13:05:56 by cado-car         ###   ########.fr       */
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
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for UNOPER command");
            return ;
        }

        // Check if the client has the necessary permissions
        if (!client->is_oper()) {
            client->reply(ERR_NOPRIVILEGES, ":Permission Denied");
            return ;
        }

        std::string target_name = message->get_params()[0];
        Client  *target = _server->get_client_by_nickname(target_name);
        if (target) {
            if (!target->is_oper()) {
                client->reply(ERR_NOTOPER, ":" + target_name + " is not an IRC operator");
                return ;
            }
            target->unOper();
            if (client->get_nickname() != target_name)
                client->reply(ERR_NOTOPER, ":" + target_name + " is no longer an IRC operator");
            target->reply(ERR_NOTOPER, ":You are no longer an IRC operator");
        } else
            client->reply(ERR_NOSUCHNICK, ":No such nick/channel");
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
}