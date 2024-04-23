/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dofranci <dofranci@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-04-22 15:31:11 by dofranci          #+#    #+#             */
/*   Updated: 2024-04-22 15:31:11 by dofranci         ###   ########.fr       */
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
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters, please provide a channel name");
            return;
        }
        std::string channel_name = message->get_params()[0];
        if (channel_name[0] != '#') {
            client->reply(ERR_NOSUCHCHANNEL, channel_name, ":No such channel");
            return;
        }
        Channel *channel = _server->get_channel(channel_name);
        
        // Check if channel exists
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, channel_name, ":No such channel");
            return ;
        }

        // Check if client is on channel
        if (channel->get_clients_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOTONCHANNEL, client->get_nickname() + channel_name, ":You're not on that channel");
            return ;
        }

        // Check if the client has the necessary permissions to change the mode
        if (!client->is_oper() && channel->get_chanop_names().find(client->get_nickname()) == std::string::npos) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, _name, ":Permission Denied");
            return ;
        }

        Client *bot = _server->get_client_by_nickname("marvin_bot");
        if(bot == NULL) {
            return;
        }
        channel->join(bot);
        std::string join_message = ":" + bot->get_nickname() + "!~" + bot->get_username() + "@" + bot->get_hostname() + " JOIN " + channel->get_name();
        channel->broadcast(bot, join_message);
        channel->set_bot(true);
    }
    return ;
}
