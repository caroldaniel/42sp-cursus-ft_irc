/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 21:40:27 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 11:51:58 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Pass::Pass(void) : Command("PASS") {
    return ;
}

Pass::~Pass(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Pass::invoke(Client *client, Message *message) {
    // Check if message has enough parameters
    if (message->get_params().size() < 1) {
        client->send_reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
        return ;
    }
    
    client->authenticate(message->get_params()[0]);
    if (!client->is_authenticated())
        client->send_reply(ERR_PASSWDMISMATCH, _name, ":Password incorrect");
    else if (client->is_registered()) 
        client->send_reply(RPL_WELCOME, "", ":Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());
    return ;
}