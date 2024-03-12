/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 21:40:27 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/11 21:47:43 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Pass::Pass(void) : Command() {
    return ;
}

Pass::~Pass(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Pass::invoke(Client *client, Message *message) {
    std::string password;

    if (message->get_params().size() < 1) {
        client->send_reply("461", "PASS :Not enough parameters");
        return ;
    }
    password = message->get_params()[0];
    client->authenticate(password);
    if (!client->is_authenticated()) {
        client->send_reply("464", "Password incorrect");
    }
    else if (client->is_authenticated() && client->is_registered()) {
        client->send_reply("001", "Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());
    }
    return ;
}