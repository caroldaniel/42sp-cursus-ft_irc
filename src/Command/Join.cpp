/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:00:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/06 23:24:01 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Join::Join(Server *server) : Command("JOIN", server) {
    return ;
}

Join::~Join(void) {
    return ;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

void Join::invoke(Client *client, Message *message) {
    if (client->is_authenticated() && client->is_registered()) {
        if (message->get_params().size() < 1) {
            client->reply(ERR_NEEDMOREPARAMS, _name, ":Not enough parameters");
            return;
        }
        
        std::string channel_name = message->get_params()[0];
        // Check if channel name has IP address after ':'. If so, remove it.
        if (channel_name.find(":") != std::string::npos)
            channel_name = channel_name.substr(0, channel_name.find(":"));

        // Check if channel has a valid name
        if (channel_name[0] != '#') {
            client->reply(ERR_NOSUCHCHANNEL, channel_name, ":No such channel");
            return;
        }

        // Check if channel exists
        Channel *channel = _server->get_channel(channel_name);
        if (channel == NULL) {
            channel = new Channel(channel_name);
            _server->add_channel(channel);
            channel->add_chanop(client);
        }
        else if (channel->get_invite_only() && channel->get_invited_names().find(client->get_nickname()) == std::string::npos) {
                client->reply(ERR_INVITEONLYCHAN, channel->get_name(), ":Cannot join channel (+i)");
                return;
        }
        else if (channel->get_channel_key() != "") {
            std::string attempted_key = message->get_params()[1];
            if (message->get_params().size() < 2) {
                client->reply(ERR_NEEDMOREPARAMS, channel->get_name(), ":Cannot join channel (+k)");
                return;
            }
            if (attempted_key != channel->get_channel_key()) {
                client->reply(ERR_BADCHANNELKEY, channel->get_name(), ":Cannot join channel (+k)");
                return;
            }
        }
        channel->join(client);

        // JOIN message
        std::string join_message = ":" + client->get_nickname() + "!~" + client->get_username() + "@" + client->get_hostname() + " JOIN " + channel->get_name();
        send(client->get_socket(), join_message.c_str(), join_message.size(), 0);
        // Channel Topic
        if (channel->get_topic() == "") {
            client->reply(RPL_NOTOPIC, "", client->get_nickname() + " " + channel->get_name() + " :No topic is set");
        } else {
            client->reply(RPL_TOPIC, "", channel->get_name() + " :" + channel->get_topic());
        }
        // Names of the users that joined the channel
        client->reply(RPL_NAMREPLY, "", "= " + channel->get_name() + " :" + channel->get_clients_names());
        client->reply(RPL_ENDOFNAMES, "", channel->get_name() + " :End of /NAMES list");

        // Broadcast JOIN message to all clients in the channel
        join_message = "has joined " + channel->get_name();
        channel->broadcast(client, join_message);
        
    }
    return;
}
