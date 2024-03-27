/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dofranci <dofranci@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-03-14 00:39:49 by dofranci          #+#    #+#             */
/*   Updated: 2024-03-14 00:39:49 by dofranci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Mode::Mode(Server *server) : Command("MODE", server) {
    return ;
}

Mode::~Mode(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Mode::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        // Check if message has enough parameters
        if (message->get_params().size() == 0) {
            client->reply(ERR_NEEDMOREPARAMS, _name, ": Not enough parameters");
            return ;
        }

        // Check if is a valid channel
        std::string channel_name = message->get_params()[0];
        if (channel_name[0] != '#') {
            client->reply(ERR_NOSUCHCHANNEL, channel_name, ": No such channel");
            return ;
        }

        size_t pos = channel_name.find(":");
        if (pos != std::string::npos) {
            channel_name = channel_name.substr(0, pos);
        }
        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, channel_name, ": No such channel");
            return ;
        }

        // Check if client is on channel
        if(channel->get_clients_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOTONCHANNEL, client->get_nickname() + channel_name, ": You're not on that channel");
            return ;
        }

        // Check if the client has the necessary permissions to change the mode
        if (!client->is_oper() && channel->get_chanop_names().find(client->get_nickname()) == std::string::npos) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, _name, ": Permission Denied");
            return ;
        }

        // Perform the necessary actions based on the mode
        std::string mode_notice = "";

        if (!channel->set_mode(message)) {
            // Send an error message to the client
            client->reply(ERR_NOSUCHNICK, "", ":No such nick/channel");
            return ;
        }
        client->reply(RPL_CHANNELMODEIS, " MODE ", message->get_params()[0] + " " + message->get_params()[1]);
        mode_notice = ":" + client->get_nickname() + " MODE " + channel->get_name() + " " + message->get_params()[0] + " " + message->get_params()[1] + "\n";

        // Send a response to the client indicating the mode change
        channel->broadcast(client, mode_notice);
    }

    return ;
}