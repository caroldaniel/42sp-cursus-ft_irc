/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 00:39:49 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/26 13:00:29 by cado-car         ###   ########.fr       */
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
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for MODE command");
            return ;
        }

        std::string channel_name = message->get_params()[0];
        
        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, ":No channel " + channel_name);
            return ;
        }

        // Check if client is on channel
        if (!channel->has_client(client)) {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel_name);
            return ;
        }

        // Check if command was invoked only for listing the channel's modes
        if (message->get_params().size() == 1) {           
            client->reply(RPL_CHANNELMODEIS, channel->get_name() + SPACE + channel->get_modes());
            return ;
        }
        
        // Check if the client has the necessary permissions to change the mode (Oper or ChanOp)
        if (!client->is_oper() && !channel->is_chanop(client->get_nickname())) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, channel->get_name() + SPACE + ":Permission Denied");
            return ;
        }

        // Check if flag only has 2 characters
        std::string mode_flag = message->get_params()[1];
        if (mode_flag.length() != 2) {
            client->reply(ERR_UNKNOWNMODE, channel->get_name() + SPACE + ":Unknown mode char");
            return ;
        }
        
        // Separate flag into two chars
        char mode = mode_flag[0];
        char flag = mode_flag[1];
        
        // Set or unset the mode
        if (mode == '-')
            channel->unset_mode(flag, message->get_params(), client, channel->get_name());
        else if (mode == '+')
            channel->set_mode(flag, message->get_params(), client, channel->get_name());
        else
            client->reply(ERR_UNKNOWNMODE, channel->get_name() + SPACE + ":Unknown mode char");
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
