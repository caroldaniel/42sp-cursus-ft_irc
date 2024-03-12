/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:16:12 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 16:35:33 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

List::List(Server *server) : Command("LIST", server) {
    return ;
}

List::~List(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    List::invoke(Client *client, Message *message) {
    if (message->get_params().size() > 0) {
        client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
        return ;
    }
    if (client->is_authenticated() && client->is_registered()) {
        client->reply(RPL_LISTSTART, "", ":Channel Users Name");
        std::vector<Channel *> channels = _server->list_channels();
        if (channels.size() != 0) {
            for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
                std::string channel_name = (*it)->get_name();
                std::stringstream user_count;
                user_count << (*it)->get_clients().size();
                std::string channel_topic = (*it)->get_topic();                            
                client->reply(RPL_LIST, "", channel_name + SPACE + user_count.str() + " :" + channel_topic);
            }
        }
        client->reply(RPL_LISTEND, "", ":End of /LIST");
        
    }
    return ;
}