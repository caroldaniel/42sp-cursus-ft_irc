/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 18:50:38 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/07 18:54:56 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Cap::Cap(Server *server) : Command("CAP", server) {
    return ;
}

Cap::~Cap(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Cap::invoke(Client *client, Message *message) {
    (void)message;
    client->reply(RPL_CAP, client->get_nickname(), "CAP :No supported capabilities available");
    return ;
}
