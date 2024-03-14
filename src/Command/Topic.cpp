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

void    Topic::invoke(Client *client, Message *message) {
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
    Channel *channel = _server->get_channel(channel_name);
    if (channel == NULL) {
        client->reply(ERR_NOSUCHCHANNEL, channel_name, ": No such channel");
        return ;
    }
    // Check if client is channel operator
    if(client->is_oper() == 0) {
        client->reply(ERR_CHANOPRIVSNEEDED, "", client->get_nickname() + channel->get_name() + ": You're not channel operator"); 
        return ;
    }
    // Show topic of channel
    if (message->get_params().size() == 1){
        if(channel->get_topic() == "No topic")
            client->reply(RPL_NOTOPIC, "", client->get_nickname() + channel->get_name() + ": No topic is set");
        else
            client->reply(RPL_TOPIC, "", channel->get_name() + " :" + channel->get_topic());
    }
    // Set topic of channel
    if (message->get_params().size() == 2) {
        std::string topic = message->get_params()[1];
        size_t pos = topic.find(":");
        if (pos != std::string::npos) {
            topic = topic.substr(0, pos);
        }
        channel->set_topic(topic);
    }
}