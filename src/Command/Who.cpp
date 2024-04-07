/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 17:47:54 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/07 19:14:18 by cado-car         ###   ########.fr       */
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
            client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
            return;
        }
    
        std::string channel_name = message->get_params()[0];
    
        // Check if channel name has IP address after ':'. If so, remove it.
        if (channel_name.find(":") != std::string::npos)
            channel_name = channel_name.substr(0, channel_name.find(":"));

        std::vector<Client *> clients = _server->list_clients();
        std::string mask = message->get_params().size() > 1 ? message->get_params()[1] : "";

        // Check if mask is valid
        if (mask != "" && mask != "o") {
            client->reply(ERR_UNKNOWNMODE, mask, ":is unknown mode char to me");
            return ;
        }

        // Get channel by name
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            client->reply(ERR_NOSUCHCHANNEL, channel_name, ":No such channel");
            return ;
        }

        // Get channels clients
        std::vector<Client *> channel_clients = channel->get_clients();

        // Check if client is on the channel. If not, return
        if (channel->get_clients_names().find(client->get_nickname()) == std::string::npos) {
            client->reply(ERR_NOTONCHANNEL, client->get_nickname() + " " + channel_name, ":You're not on that channel");
            return ;
        }        

        // Loop through clients on channel and reply with WHO information
        for (std::vector<Client *>::iterator it = channel_clients.begin(); it != channel_clients.end(); it++) {
            if (mask == "o" && (!(*it)->is_oper() && channel->get_chanop_names().find((*it)->get_nickname()) == std::string::npos))
                continue ;
            std::string message = "";
            message += (*it)->get_nickname();
            message += " " + (*it)->get_username();
            message += " " + (*it)->get_hostname();
            message += " " + _server->get_hostname();
            message += " " + (*it)->get_nickname();
            message += " ";
            if ((*it)->is_oper())
                message += "+O";
            if (channel->is_chanop((*it)->get_nickname()))
                message += "@";
            if (!(*it)->is_oper() && !channel->is_chanop((*it)->get_nickname()))
                message += "0";
            message += " :" + (*it)->get_realname();
            client->reply(RPL_WHOREPLY, "", message);
        }
        client->reply(RPL_ENDOFWHO, mask, channel_name + " :End of WHO list");
    }
    return ;
}
