/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 20:46:25 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 18:49:30 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Privmsg::Privmsg(Server *server) : Command("PRIVMSG", server) {
    return ;
}

Privmsg::~Privmsg(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Privmsg::invoke(Client *client, Message *message) {
    std::string target_name;
    std::string message_text;
    
    if (client->is_authenticated() && client->is_registered()) {
        // Check if message has enough parameters
        if (message->get_params().size() < 2) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for PRIVMSG command");
            return ;
        }
        target_name = message->get_params()[0];
        message_text = message->get_params()[1];

        // Try to get target by name
        Channel *channel = _server->get_channel(target_name);
        Client  *target = _server->get_client(target_name);
        
        // Check if target is neither a channel nor a user
        if (!channel && !target) {
            client->reply(ERR_NOSUCHNICK, ":No such nick/channel");
            return ;
        }
        
        // If target is a channel
        if (channel) {
            // Check if client is on channel
            if (!channel->has_client(client)) {
                client->reply(ERR_NOTONCHANNEL, ":You're not on channel " + target_name);
                return ;
            }
            channel->broadcast(client, _name, "", message_text);
            if (check_bot_command(message_text, channel)) 
                bot_reply(client, channel, message_text);
        }
        // If target is a user
        else {
            // Check if target is marvin_bot
            if (target->get_nickname() == "marvin_bot") 
                bot_reply(client, NULL, message_text);
            else
                target->broadcast(client, _name, target_name, message_text);
        }
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
