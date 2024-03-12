/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 23:26:36 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 16:42:00 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

Quit::Quit(Server *server) : Command("QUIT", server) {
    return ;
}

Quit::~Quit(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    Quit::invoke(Client *client, Message *message) {
    // Placeholder for message parameter before broadcast function is ready
    message = message;
    // Send goodbye message to client
    client->reply(RPL_QUIT, _name, ":Thank you for using the IRC Network, " + client->get_nickname() + ". Goodbye!");
    client->disconnect();
    return ;
}