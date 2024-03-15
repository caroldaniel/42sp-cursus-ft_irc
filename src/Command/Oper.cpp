/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:00:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 19:14:31 by cado-car         ###   ########.fr       */
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
    if (message->get_params().size() < 2) {
        client->reply(ERR_NEEDMOREPARAMS, _name, ": Not enough parameters");
        return ;
    }

    std::string target_name = message->get_params()[0];
    size_t pos = target_name.find(":");
    if (pos != std::string::npos) {
        target_name = target_name.substr(0, pos);
    }

    Client *target = _server->get_client_by_nickname(target_name);
    if (target) {
        if (target->is_registered()) {
            target->oper(message->get_params()[1]);
            if (target->is_oper()) {
                client->reply(RPL_YOUREOPER, "", target->get_nickname() + ": You are now an IRC operator");
            } else {
                client->reply(ERR_PASSWDMISMATCH, "", target->get_nickname() + ": Password incorrect");
            }
        }
    } else {
        client->reply(ERR_NOSUCHNICK, message->get_params()[0], ": No such nick/channel");
    }
}
