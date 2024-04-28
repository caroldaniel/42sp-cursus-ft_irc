/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:36:12 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/28 19:27:52 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                         Constructors and Destructor                        */
/******************************************************************************/

User::User(Server *server) : Command("USER", server) {
    return ;
}

User::~User(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void    User::invoke(Client *client, Message *message) {

    std::string username;
    std::string realname;

    // Check if the user is already registered
    if (client->is_registered()) {
        client->reply(ERR_ALREADYREGISTRED, ":You may not reregister");
        return ;
    }

    // Check if client has a nickname
    if (message->get_params().size() < 4) {
        client->disconnect("Server disconnected due to registration failure");
        return ;
    }
    
    // Check if username is composed of valid characters (alpha-numeric, underscore, dash and dot only)
    username = message->get_params()[0];
    for (size_t i = 0; i < username.length(); i++) {
        if(!std::isalnum(username[i]) && username[i] != '_' && username[i] != '-' && username[i] != '.') {
            client->reply(ERR_ERRONEUSUSERNAME, ":Invalid username. Use only alpha-numeric characters, underscore, dash and dot.");
            client->disconnect("Server disconnected due to registration failure");
            return ;
        }
    }

    // Check if realname is composed of valid characters (any printable character - including spaces - is allowed)
    realname = message->get_params()[3];
    for (size_t i = 0; i < realname.length(); i++) {
        if(!std::isprint(realname[i])) {
            client->reply(ERR_ERRONEUSREALNAME, ":Invalid realname. Use only printable characters.");
            client->disconnect("Server disconnected due to registration failure");
            return ;
        }
    }
    
    // Register the user
    client->set_username(username);
    client->set_realname(realname);
    client->register_client();
    return ;
}