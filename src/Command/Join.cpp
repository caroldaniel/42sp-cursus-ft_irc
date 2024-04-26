/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:00:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/25 22:46:20 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Join::Join(Server *server) : Command("JOIN", server) {
    return ;
}

Join::~Join(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Join::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for JOIN command");
            return;
        }
        std::string channel_name = message->get_params()[0];
        if (channel_name[0] != '#') {
            client->reply(ERR_NOSUCHCHANNEL, ":No such channel [" + channel_name + "]");
            return;
        }

        Channel *channel = _server->get_channel(channel_name);
        // Create channel if it does not exist. The client is the first user to join.
        if (channel == NULL) {
            channel = new Channel(channel_name, _server->get_hostname());
            _server->add_channel(channel);
            channel->join(client);
            channel->add_chanop(client);
        } else {
            if (channel->get_invite_only() && channel->get_invited_names().find(client->get_nickname()) == std::string::npos) {
                client->reply(ERR_INVITEONLYCHAN, channel->get_name() + SPACE + ":Cannot join channel (+i)");
                return;
            }
            if(channel->has_key() && message->get_params()[1] != channel->get_key()) {
                client->reply(ERR_BADCHANNELKEY, channel->get_name() + SPACE + ":Cannot join channel (+k)");
                return;
            }
            if(channel->has_user_limit() && channel->get_user_quantity() >= channel->get_user_limit()) {
                client->reply(ERR_CHANNELISFULL, channel->get_name() + SPACE + ":Cannot join channel (+l)");
                return;
            }
            channel->join(client);
        }

        // Channel Topic
        std::vector<std::string> params;
        params.push_back(channel->get_name());
        channel->topic(client, params);
        
        // Channel Names
        channel->update_list_names();
    }
    else {
        client->reply(ERR_NOTREGISTERED, " :You have not registered");
    }
    return;
}