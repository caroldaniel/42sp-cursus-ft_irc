/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:36:12 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/11 21:36:41 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

User::User(void) : Command() {
    return ;
}

User::~User(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    User::invoke(Client *client, Message *message) {
    if (message->get_params().size() < 4) {
        client->send_reply("461", "USER :Not enough parameters");
        return ;
    }
    client->set_username(message->get_params()[0]);
    client->set_realname(message->get_params()[3]);
    if (client->is_authenticated()) {
        client->send_reply("001", "Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());
    }    
    return ;
}