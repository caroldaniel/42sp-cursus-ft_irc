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

        // Check if client is on channel
        if(channel->get_clients_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOTONCHANNEL, client->get_nickname() + channel_name, ":You're not on that channel");
            return ;
        }

        // Check if client has permission to change topic
        if(channel->get_topic_restriction()) {
            if (!client->is_oper() && channel->get_chanop_names().find(client->get_nickname()) == std::string::npos) {  
                client->reply(ERR_CHANOPRIVSNEEDED, channel_name, ":You're not channel operator");
                return ;
            }
        }
        // If no topic is set, return RPL_NOTOPIC
        if (message->get_params().size() == 1) {
            if (channel->get_topic() == "") {
                client->reply(RPL_NOTOPIC, channel_name, ":No topic is set");
            } else {
                client->reply(RPL_TOPIC, channel_name, ":" + channel->get_topic());
            }
        }

        // Set topic of channel
        if (message->get_params().size() == 2) {
            std::string topic = message->get_params()[1];
            channel->set_topic(topic);
            channel->broadcast(client, message->get_prefix() + "TOPIC " + channel_name + " :" + topic);
        }
    }
}
