/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 15:31:11 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/26 18:32:20 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Bot::Bot(Server *server) : Command("BOT", server) {
    return ;
}

Bot::~Bot(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Bot::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        // Check if the command has the necessary parameters
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for BOT command");
            return;
        }
        
        std::string channel_name = message->get_params()[0];
        Channel *channel = _server->get_channel(channel_name);

        // Check if channel exists
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, ":No channel " + channel_name);
            return ;
        }

        // Check if client is on channel
        if (!channel->has_client(client)) {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel_name);
            return ;
        }

        // Check if the client has the necessary permissions to change the mode
        if (!channel->is_chanop(client->get_nickname())) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, ":Permission Denied");
            return ;
        }

        Client *bot = _server->get_client("marvin_bot");
        if(bot == NULL) {
            return;
        }
        channel->join(bot);
        channel->set_bot(true);
    }
    else {
        client->reply(ERR_NOTREGISTERED, " :You have not registered");
    }
    return ;
}
