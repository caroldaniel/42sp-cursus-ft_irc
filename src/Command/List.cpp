/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:16:12 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 10:17:48 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

List::List(void) : Command() {
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
        client->send_reply("321", "Channel :Users  Name");
        client->send_reply("322", "#test :1  Test channel");
        client->send_reply("323", ":End of /LIST");
    }
    