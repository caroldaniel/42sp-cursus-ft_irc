/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 21:40:27 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/10 23:25:51 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Pass::Pass(Server *server, std::vector<std::string> tokens) : Command(server, tokens) {
    return ;
}

Pass::~Pass(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Pass::execute_command(Client *client) {
    std::string password = _tokens[1];
    client->authenticate(password);
    if (client->is_authenticated()) {
        client->send_reply("001", "Welcome to the Internet Relay Network " + client->get_username() + "!" + client->get_hostname());
    }
    else {
        client->send_reply("464", "Password incorrect");
    }
    return ;
}