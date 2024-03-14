/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 20:46:25 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 21:32:03 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Privmsg::Privmsg(Server *server) : Command("PRIVMSG", server) {
    return ;
}

Privmsg::~Privmsg(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Privmsg::invoke(Client *client, Message *message) {
    // Check if message has enough parameters
    if (message->get_params().size() < 2) {
        client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
        return ;
    }
    // Check if target is a channel or a user
    std::string target_name = message->get_params()[0];
    size_t pos = target_name.find(":");
    if (pos != std::string::npos) {
        target_name = target_name.substr(0, pos);
    }
    Channel *channel = _server->get_channel(target_name);
    if (channel) {
        // Send message to channel
        channel->broadcast(client, message->get_params()[1]);
    } else {
        // Send message to user
        Client *target = _server->get_client_by_nickname(target_name);
        if (target) {
            target->broadcast(client, target->get_nickname(), message->get_params()[1]);
        } else {
            client->reply(ERR_NOSUCHNICK, target_name, ":No such nick/channel");
        }
    }
    return ;
}

