/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 00:39:49 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/07 19:21:21 by cado-car         ###   ########.fr       */
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
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
            return ;
        }

        std::string channel_name = message->get_params()[0];

        // Check if channel name has IP address after ':'. If so, remove it.
        if (channel_name.find(":") != std::string::npos)
            channel_name = channel_name.substr(0, channel_name.find(":"));

        Channel *channel = _server->get_channel(channel_name);
        
        // Check if channel exists
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, channel_name, ":No such channel");
            return ;
        }

        // Check if client is on channel
        if(channel->get_clients_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOTONCHANNEL, client->get_nickname() + channel_name, ":You're not on that channel");
            return ;
        }

        // Check if command was invoked only with the channel name
        if (message->get_params().size() == 1) {           
            client->reply(RPL_CHANNELMODEIS, channel->get_name(), channel->get_modes());
            return ;
        }
        
        // Check if the client has the necessary permissions to change the mode
        if (!client->is_oper() && channel->get_chanop_names().find(client->get_nickname()) == std::string::npos) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, _name, ":Permission Denied");
            return ;
        }

        // Check if flag only has 2 characters
        std::string mode_flag = message->get_params()[1];        
        if (mode_flag.length() != 2) {
            client->reply(ERR_UNKNOWNMODE, mode_flag, ":Unknown mode char");
            return ;
        }
        
        // Separate flag into two chars
        char mode = mode_flag[0];
        char flag = mode_flag[1];
        
        // Set or unset the mode
        std::string message_to_broadcast = "";
        if (mode == '-')
            channel->unset_mode(flag, message->get_params(), client);
        else if (mode == '+')
            channel->set_mode(flag, message->get_params(), client);
        else {
            client->reply(ERR_UNKNOWNMODE, mode_flag, ":Unknown mode char");
            return ;
        }
    }

    return ;
}
