/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dofranci <dofranci@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-03-14 00:39:49 by dofranci          #+#    #+#             */
/*   Updated: 2024-03-14 00:39:49 by dofranci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Mode::Mode(Server *server) : Command("MODE", server) {
    return ;
}

Mode::~Mode(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Mode::invoke(Client *client, Message *message) {
        // Check if the client has the necessary permissions to change the mode
        if (!client->is_oper()) {
            // Send an error message to the client
            client->reply(ERR_NOPRIVILEGES, _name, ": Permission Denied");
            return ;
        }
        // Extract the target and mode parameters from the message
        std::string target = message->get_params()[0];
        std::string mode = message->get_params()[1];

        // Perform the necessary actions based on the mode
        if(target[0] == '#'){
            Channel *channel = _server->get_channel(target);
            if (channel) {
                // Change the mode of the channel
                channel->set_mode(target, mode);
            } else {
                // Send an error message to the client
                client->reply(ERR_NOSUCHCHANNEL, "", ": No such channel");
                return ;
            }
        } else {
                // Send an error message to the client
                client->reply(ERR_NOSUCHNICK, "", ": No such nick/channel");
                return ;
            }
                     

        // Send a response to the client indicating the mode change
        //client->reply(RPL_MODE, "Mode changed successfully.");

        // You can also broadcast the mode change to other clients if needed
        // std::string mode_notice = ":" + client->get_nickname() + "!~d" + "@" + client->get_hostname() + " JOIN " + channel->get_name();
        // channel->broadcast(client, join_notice);

        return ;
}