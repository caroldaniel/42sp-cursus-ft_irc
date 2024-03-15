/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dofranci <dofranci@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-03-15 02:15:14 by dofranci          #+#    #+#             */
/*   Updated: 2024-03-15 02:15:14 by dofranci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Part::Part(Server *server) : Command("Part", server) {
    return ;
}

Part::~Part(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Part::invoke(Client *client, Message *message) {
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
    channel->leave(client);
}
