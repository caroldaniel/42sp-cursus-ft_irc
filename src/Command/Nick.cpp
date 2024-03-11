/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 23:09:21 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/10 23:19:23 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Nick::Nick(Server *server, std::vector<std::string> tokens) : Command(server, tokens) {
    return ;
}

Nick::~Nick(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Nick::execute_command(Client *client) {
    std::string nickname = _tokens[1];
    client->set_nickname(nickname);
    client->send_reply("001", "Welcome to the Internet Relay Network " + nickname + "!" + client->get_hostname());
    return ;
}