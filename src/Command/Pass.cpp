/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 21:40:27 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/25 14:35:39 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Pass::Pass(Server *server) : Command("PASS", server) {
    return ;
}

Pass::~Pass(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Pass::invoke(Client *client, Message *message) {
    // Check if client is already authenticated'
    if (client->is_authenticated()) {
        client->reply(ERR_ALREADYREGISTRED, ":You are already authenticated");
        return ;
    }
    
    // Check if message has enough parameters
    if (message->get_params().size() < 1) {
        client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters on PASS command");
        client->disconnect("Server disconnected due to authentication failure");
        return ;
    }

    // Authenticate client
    client->authenticate(message->get_params()[0]);
    
    if (!client->is_authenticated()) {
        client->reply(ERR_PASSWDMISMATCH, ":Incorrect password");
        client->disconnect("Server disconnected due to authentication failure");
    }
        
    else if (client->is_registered())
        client->reply(RPL_WELCOME, ":Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());
    return ;
}