/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 23:06:58 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/07 19:27:52 by cado-car         ###   ########.fr       */
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
            client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
            return ;
        }

        std::string channel_name = message->get_params()[0];
        
        // Check if channel name has IP address after ':'. If so, remove it.
        if (channel_name.find(":") != std::string::npos)
            channel_name = channel_name.substr(0, channel_name.find(":"));
        
        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, channel_name, ":No such channel");
            return ;
        }

        // Check if the client is in the channel
        if(channel->get_clients_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOTONCHANNEL, channel_name, ":You're not on that channel");
            return ;
        }

        // Check if the client has the necessary permissions to kick
        if (!client->is_oper() && channel->get_chanop_names().find(client->get_nickname()) == std::string::npos) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, _name, ":Permission Denied");
            return ;
        }

        // Check if the target exists
        Client *target = _server->get_client_by_nickname(message->get_params()[1]);
        if (target == NULL) {
            client->reply(ERR_NOSUCHNICK, message->get_params()[1], ":No such nick/channel");
            return ;
        }

        // Check if the target is in the channel
        if(channel->get_clients_names().find(target->get_nickname()) == std::string::npos) {
            client->reply(ERR_USERNOTINCHANNEL, "", ":" + target->get_nickname() + " is not on " + channel->get_name());
            return ;
        }

        // Kick the target from the channel
        channel->kick(client, target, message->get_params().size() > 2 ? message->get_params()[2] : "Kicked");
    }
}
