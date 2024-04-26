/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 17:47:54 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 13:05:00 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

Who::Who(Server *server) : Command("WHO", server) {
    return ;
}

Who::~Who(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Who::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for WHO command");
            return;
        }
    
        // Get channel by name
        std::string channel_name = message->get_params()[0];
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, ":No channel " + channel_name);
            return ;
        }

        // Check if client is on the channel
        if (!channel->has_client(client)) {
            client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + channel_name);
            return ;
        }   


        std::string flag = message->get_params().size() > 1 ? message->get_params()[1] : "";

        // Check if mask is valid
        if (flag != "" && flag != "o") {
            client->reply(ERR_UNKNOWNMODE, ":Flag '" + flag + "' is unknown mode char");
            return ;
        }

        std::vector<Client *> clients;
        // List all clients on a channel
        if (message->get_params().size() == 1)
            clients = channel->get_clients();
        // List
        else
            clients = channel->get_chanop_clients();

        // Loop through clients reply with WHO information
        std::vector<Client *>::iterator it;
        for (it = clients.begin(); it != clients.end(); it++) {
            std::string info = channel->get_client_info(*it, channel);
            client->reply(RPL_WHOREPLY, channel->get_name() + SPACE + info);
        }
        client->reply(RPL_ENDOFWHO, channel->get_name() + SPACE + ":End of /WHO list");
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
