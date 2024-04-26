/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:00:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/25 22:30:28 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Oper::Oper(Server *server) : Command("OPER", server) {
    return ;
}

Oper::~Oper(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Oper::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for OPER command");
            return ;
        }
        
        std::string target_name = message->get_params()[0];
        Client *target = _server->get_client_by_nickname(target_name);

        if (target) {
            if (!target->is_registered()) {
                client->reply(ERR_NOTREGISTERED, ":" + target_name + " has not registered");
                return ;
            }
            if (target->is_oper()) {
                client->reply(RPL_YOUREOPER, ":User is already an IRC operator");
                return ;
            }
            
            // If the client is an operator, it can assign operator status to another client without a password
            if (message->get_params().size() == 1) {
                if (!client->is_oper()) {
                    client->reply(ERR_NOPRIVILEGES, ":Permission Denied");
                    return ;
                } else
                    target->oper(_server->get_oper_password());
            // Otherwise, check if the password is correct
            } else
                target->oper(message->get_params()[1]);
            
            if (target->is_oper()) {
                if (client->get_nickname() != target_name)
                    client->reply(RPL_YOUREOPER, ":" + target_name + " was assigned as a new IRC operator");
                target->reply(RPL_YOUREOPER, ":You are now an IRC operator");
            } else
                client->reply(ERR_PASSWDMISMATCH, ":Operator password is incorrect");
        } else
            client->reply(ERR_NOSUCHNICK, ":No such nick/channel");
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
