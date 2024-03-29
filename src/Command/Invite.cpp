/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dofranci <dofranci@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-03-22 18:30:04 by dofranci          #+#    #+#             */
/*   Updated: 2024-03-22 18:30:04 by dofranci         ###   ########.fr       */
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
            client->reply(ERR_NEEDMOREPARAMS, _name, ": Not enough parameters");
            return ;
        }

        // Check if is a valid channel
        std::string channel_name = message->get_params()[1];
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

        // Check if the client has the necessary permissions to kick
        if (!client->is_oper() && channel->get_chanop_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_CHANOPRIVSNEEDED, client->get_nickname() + channel->get_name() , ":You're not channel operator");
            return ;
        }

        // Check if the client is in the channel
        if(channel->get_clients_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOTONCHANNEL, channel_name, ": You're not on that channel");
            return ;
        }

        // Check if the target is in the channel
        Client *target = _server->get_client_by_nickname(message->get_params()[0]);
        if (target == NULL) {
            client->reply(ERR_NOSUCHNICK, message->get_params()[0], ": No such nick/channel");
            return ;
        }

        if(channel->get_clients_names().find(target->get_nickname()) != std::string::npos) {
           client->reply(ERR_USERONCHANNEL, "", target->get_nickname() + channel->get_name() + ": is already on channel");
        }

        // Send an invite message to the target
        channel->invite(target);
        client->reply(RPL_INVITING, "", target->get_nickname() + " " +  channel->get_name());
        std::string invite_reply = ":" + client->get_nickname() + " INVITE " + target->get_nickname() + " " + channel->get_name() + CRLF;
        std::cout << "Sending reply: " << invite_reply << std::endl;
        send(target->get_socket(), invite_reply.c_str(), invite_reply.length(), 0);

    }
}