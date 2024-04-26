/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:00:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 19:14:31 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Topic::Topic(Server *server) : Command("Topic", server) {
    return ;
}

Topic::~Topic(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Topic::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        // Check if message has enough parameters
        if (message->get_params().size() == 0) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for TOPIC command");
            return ;
        }

        std::string channel_name = message->get_params()[0];
        std::string client_name = client->get_nickname();
        
        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, ":No channel " + channel_name);
            return ;
        }

        // Check if client is on channel
        if(!channel->has_client(client)) {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel_name);
            return ;
        }

        // Check if client has permission to change topic
        if(message->get_params().size() == 2 && channel->get_topic_restriction() && !channel->is_chanop(client_name)) {
            client->reply(ERR_CHANOPRIVSNEEDED, channel->get_name() + SPACE + "Cannot set topic. You're not channel operator");
            return ;
        }

        channel->topic(client, message->get_params());
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
}
