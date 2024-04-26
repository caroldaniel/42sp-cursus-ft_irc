/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:16:12 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 13:02:39 by cado-car         ###   ########.fr       */
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
    if (client->is_authenticated() && client->is_registered()) {
        (void)message;
        client->reply(RPL_LISTSTART, ":Channel Users Topic");
        std::vector<Channel *> channels = _server->list_channels();
        if (channels.size() != 0) {
            std::string list_content;
            for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
                std::string channel_name = (*it)->get_name();
                std::stringstream user_count;
                user_count << (*it)->get_clients().size();
                std::string channel_topic = (*it)->get_topic();
                list_content = channel_name + SPACE + user_count.str() + SPACE + channel_topic;                        
                client->reply(RPL_LIST, list_content);
            }
        }
        client->reply(RPL_LISTEND, ":End of /LIST");
    } else {
        client->reply(ERR_NOTREGISTERED, ":You have not registered");
    }
    return ;
}