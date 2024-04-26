/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 17:47:54 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/25 21:30:47 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

Names::Names(Server *server) : Command("NAMES", server) {
    return ;
}

Names::~Names(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Names::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        // Check if message has enough parameters
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for NAMES command");
            return ;
        }

        std::string channel_name = message->get_params()[0];
        
        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, ":No such channel [" + channel_name + "]");
            return ;
        }

        // Check if client is on channel
        if (!channel->has_client(client)) {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel->get_name());
            return ;
        }

        channel->names(client);
    }
    else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
