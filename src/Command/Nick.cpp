/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 23:09:21 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/11 21:36:33 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Nick::Nick(void) : Command() {
    return ;
}

Nick::~Nick(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Nick::invoke(Client *client, Message *message) {
    if (message->get_params().size() < 1) {
        client->send_reply("431", "No nickname given");
        return ;
    }
    client->set_nickname(message->get_params()[0]);
    if (client->is_authenticated() && client->is_registered()) {
        client->send_reply("001", "Welcome to the Internet Relay Network " + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname());
    }
    return ;
}
