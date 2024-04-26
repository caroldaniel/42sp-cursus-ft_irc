/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 02:15:14 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/25 22:07:31 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Part::Part(Server *server) : Command("Part", server) {
    return ;
}

Part::~Part(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Part::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {

        // Check if message has enough parameters
        if (message->get_params().size() == 0) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for PART command");
            return ;
        }

        std::string channel_name = message->get_params()[0];

        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, ":No such channel");
            return ;
        }
        
        // Check if client is on channel
        if (!channel->has_client(client)) {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel_name);
            return ;
        }

        channel->part(client, message->get_params().size() > 1 ? message->get_params()[1] : "");
    }
    else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
