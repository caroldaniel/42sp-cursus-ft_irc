/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:30:04 by dofranci          #+#    #+#             */
/*   Updated: 2024/04/26 15:46:19 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Invite::Invite(Server *server) : Command("INVITE", server) {
    return ;
}

Invite::~Invite(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Invite::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        // Check if message has enough parameters
        if (message->get_params().size() < 2) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for INVITE command");
            return ;
        }

        std::string channel_name = message->get_params()[1];
        
        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, ":No channel " + channel_name);
            return ;
        }

        // Check if the client is in the channel
        if(channel->get_client_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel_name);
            return ;
        }

        // Check if the client has the necessary permissions to invite
        if (!client->is_oper() && !channel->is_chanop(client->get_nickname())) {
            client->reply(ERR_CHANOPRIVSNEEDED, ":You're not channel " + channel_name + " operator");
            return ;
        }

        Client *target = _server->get_client(message->get_params()[0]);
        
        // Check if the target exists
        if (target == NULL) {
            client->reply(ERR_NOSUCHNICK, ":No such nick/channel");
            return ;
        }
        
        // Check if the target is in the channel
        if(channel->has_client(target)) {
           client->reply(ERR_USERONCHANNEL, ":" + target->get_nickname() + " is already on channel " + channel_name);
        }

        // Send an invite message to the target
        channel->invite(target);
        client->reply(RPL_INVITING, target->get_nickname() + " " +  channel->get_name());
        target->broadcast(client, "INVITE", target->get_nickname(), channel->get_name());
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}