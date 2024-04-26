/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 17:47:54 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 17:24:12 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

Whois::Whois(Server *server) : Command("WHOIS", server) {
    return ;
}

Whois::~Whois(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Whois::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, ":Not enough parameters for WHOIS command");
            return;
        }
    
        // Get client by nickname
        std::string target_name = message->get_params()[0];
        Client *target = _server->get_client(target_name);
        if (target == NULL) {
            client->reply(ERR_NOSUCHNICK, ":No such nick/channel");
            return ;
        }

        // Reply with user information
        client->reply(RPL_WHOISUSER, target->get_nickname() + SPACE + target->get_username() + SPACE + target->get_hostname() + SPACE + "*" + SPACE + ":" + target->get_realname());
        client->reply(RPL_WHOISSERVER, target->get_nickname() + SPACE + target->get_hostname() + SPACE + ":" + _server->get_info());
        if (target->is_oper())
            client->reply(RPL_WHOISOPERATOR, target->get_nickname() + SPACE + ":is an IRC operator");
        client->reply(RPL_ENDOFWHOIS, target->get_nickname() + SPACE + ":End of WHOIS list");        
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}
