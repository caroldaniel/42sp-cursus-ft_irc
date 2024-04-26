/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 20:46:25 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 13:03:44 by cado-car         ###   ########.fr       */
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
        Client  *target = _server->get_client_by_nickname(target_name);
        
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
            // if (message->get_params()[1].find("!") != std::string::npos && channel->has_bot() == true) {
            //     std::vector<Client *>   clients = channel->get_clients();
            //     Client *bot = channel->get_client_by_nickname("marvin_bot", clients);
            //     if(bot != NULL)
            //     {
            //         std::string command = message->get_params()[1].substr(1);
            //         channel->broadcast(client, message->get_params()[1]);
            //         if (command == "time")
            //             channel->broadcast(bot, "The time is now: " + get_current_time());
            //         else if(command == "commands")
            //             channel->broadcast(bot, "Available commands: time, date, joke, quit");
            //         else if (command == "date")
            //             channel->broadcast(bot, "Today is: " + get_current_date());
            //         else if (command == "joke")
            //             channel->broadcast(bot, "Here is a joke: " + get_random_joke());
            //         else if (command == "quit")
            //         {
            //             if (!client->is_oper() && channel->get_chanop_names().find(client->get_nickname()) == std::string::npos) {
            //                 channel->broadcast(bot, "You can't make me leave!");
            //                 return ;
            //             }
            //             channel->broadcast(bot, "Goodbye, cruel world!");
            //             channel->set_bot(false);
            //         }
            //         else
            //             channel->broadcast(bot, "I'm sorry, I don't understand that command. Try !commands for a list of available commands.");
            //     }
            // }
            channel->broadcast(client, _name, "", message_text);
        }
        
        // If target is a user
        else
            target->broadcast(client, _name, target_name, message_text);
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
