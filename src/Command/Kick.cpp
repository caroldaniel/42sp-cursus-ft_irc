/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 23:06:58 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/26 15:46:19 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Kick::Kick(Server *server) : Command("KICK", server) {
    return ;
}

Kick::~Kick(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Kick::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        // Check if message has enough parameters
        if (message->get_params().size() < 2) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for KICK command");
            return ;
        }

        std::string channel_name = message->get_params()[0];
        
        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, ":No channel " + channel_name);
            return ;
        }

        // Check if the client is in the channel
        if(!channel->has_client(client)) {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel_name);
            return ;
        }

        // Check if the client has the necessary permissions to kick
        if (!client->is_oper() && channel->get_chanop_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOPRIVILEGES, channel->get_name() + SPACE + ":Permission Denied");
            return ;
        }

        // Check if the target exists
        Client *target = _server->get_client(message->get_params()[1]);
        if (target == NULL) {
            client->reply(ERR_NOSUCHNICK, channel->get_name() + SPACE + ":No such nick/channel");
            return ;
        }

        // Check if the target is in the channel
        if(!channel->has_client(target)) {
            client->reply(ERR_USERNOTINCHANNEL, channel->get_name() + SPACE + ":" + target->get_nickname() + " is not on " + channel->get_name());
            return ;
        }

        // Kick the target from the channel
        std::string reason = message->get_params().size() > 2 ? message->get_params()[2] : "Kicked";
        channel->kick(client, target, reason);
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
}
