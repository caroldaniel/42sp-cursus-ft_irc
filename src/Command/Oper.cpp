/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:00:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/06 21:02:31 by cado-car         ###   ########.fr       */
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
        if (message->get_params().size() < 2) {
            client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
            return ;
        }
        
        std::string target_name = message->get_params()[0];
        if (client->get_nickname() != target_name && !client->is_oper()) {
            client->reply(ERR_NOPRIVILEGES, "", ":Permission Denied- You're not an IRC operator");
            return ;
        }
        Client *target = _server->get_client_by_nickname(target_name);
        if (target) {
            if (client->is_authenticated() && target->is_registered() && !target->is_oper())  {
                target->oper(message->get_params()[1]);
                if (target->is_oper()) {
                    if (client->get_nickname() != target_name)
                        client->reply(RPL_YOUREOPER, target_name, ":Was assigned as a new IRC operator");
                    target->reply(RPL_YOUREOPER, client->get_nickname(), ":You are now an IRC operator");
                } else {
                    client->reply(ERR_PASSWDMISMATCH, "", ":Password incorrect");
                }
            }
        } else {
            client->reply(ERR_NOSUCHNICK, message->get_params()[0], ":No such nick/channel");
        }
    }
}
