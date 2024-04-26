/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 09:47:16 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 16:53:52 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Channel::Channel(std::string name, std::string hostname) 
    : _name(name),
    _hostname(hostname),
    _topic(""), 
    _key(""), 
    _user_limit(0),
    _user_quantity(0),
    _topic_restriction(true), 
    _invite_only(false), 
    _has_key(false), 
    _has_bot(false),
    _has_user_limit(false) {
    return ;
}

Channel::Channel(const Channel &other) {
    *this = other;
}

Channel::~Channel(void) {
    // Clear the clients vector
    this->_clients.clear();
    return ;
}

/******************************************************************************/
/*                            Operator Overload                               */
/******************************************************************************/

Channel &Channel::operator=(const Channel &other) {
    *this = other;
    return *this;
}

/******************************************************************************/
/*                     Member Functions: Channel commands                     */
/******************************************************************************/

void    Channel::join(Client *client) {
    this->_clients.push_back(client);
    this->_user_quantity++;
    this->broadcast(client, "JOIN", this->get_name(), "");
    return ;
}

void    Channel::part(Client *client, std::string message) {
    this->remove_chanop(client);
    this->broadcast(client, "PART", this->get_name(), message);
    this->leave(client);
    return ;
}

void    Channel::quit(Client *client, std::string message) {
    this->remove_chanop(client);
    this->broadcast(client, "QUIT", this->get_name(), message);
    this->leave(client);
    return ;
}

void    Channel::topic(Client *client, std::vector<std::string> params) {
    // Get channel topic
    if (params.size() == 1) {
        if (this->get_topic().empty()) {
            client->reply(RPL_NOTOPIC, this->get_name() + SPACE + ":No topic is set");
        } else {
            client->reply(RPL_TOPIC, this->get_name() + SPACE + this->get_topic());
        }
    }
    // Set channel topic
    if (params.size() == 2) {
        std::string topic = params[1];
        this->set_topic(topic);
        this->broadcast(client, "TOPIC", this->get_name(), topic);
    }
    return ;
}

void    Channel::names(Client *client) {
    client->reply(RPL_NAMREPLY, "= " + this->get_name() + " :" + this->get_client_names());
    client->reply(RPL_ENDOFNAMES, this->get_name() + " :End of /NAMES list");
    return ;
}

void    Channel::mode(Client *client, std::string message) {
    this->broadcast(client, "MODE", this->get_name(), message);
    return ;
}


void    Channel::kick(Client *client, Client *target, std::string reason) {

    if (client != target && !is_chanop(client->get_nickname())) {
        client->reply(ERR_CHANOPRIVSNEEDED, this->get_name() + SPACE + ":You're not channel operator");
        return ;    
    }
    std::string message = target->get_nickname() + " :" + reason;

    this->broadcast(client, "KICK", this->get_name(), message);
    this->leave(target);
    return ;
}

/******************************************************************************/
/*                   Member Functions: Operator management                    */
/******************************************************************************/

void    Channel::add_chanop(Client *client) {
    this->_op_clients.push_back(client);
    return ;
}

void    Channel::remove_chanop(Client *client) {
    std::vector<Client *>::iterator it = std::find(this->_op_clients.begin(), this->_op_clients.end(), client); 
    if (it != this->_op_clients.end())
        this->_op_clients.erase(it);
    return ;
}

bool    Channel::is_chanop(std::string nickname) {
    for (std::vector<Client *>::iterator it = this->_op_clients.begin(); it != this->_op_clients.end(); it++)
        if ((*it)->get_nickname() == nickname)
            return true;
    return false;
}

/******************************************************************************/
/*                   Member Functions: Channel management                     */
/******************************************************************************/

void    Channel::invite(Client *client) {
    this->_invited_clients.push_back(client);
    return ;
}   

void    Channel::leave(Client *client) {
    // Remove client from channel operators
    std::vector<Client *>::iterator it_op = std::find(this->_op_clients.begin(), this->_op_clients.end(), client);
    if (it_op != this->_op_clients.end())
        this->_op_clients.erase(it_op);

    // Remove client from channel
    std::vector<Client *>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), client); 
    if (it != this->_clients.end()) {
        this->_clients.erase(it);
        this->_user_quantity--;
    }
    return ;
}

void     Channel::set_mode(char mode, std::vector<std::string> params, Client *client, std::string channel_name) {
    
    std::string message = "";
    Client      *target;
    std::string target_name;
    std::string limit_param;
    int         limit;

    switch (mode) {
        case 't':
            _topic_restriction = true;
            message = "+t";
            break;
        case 'i':
            _invite_only = true;
            message = "+i";
            break;
        case 'o':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, channel_name + SPACE + ":Not enough parameters");
                break ;
            }
            target_name = params[2];
            target = get_client(target_name, _clients);
            if (!this->has_client(target)) {
                client->reply(ERR_USERNOTINCHANNEL, channel_name + SPACE + ":User is not on this channel");
                break ;
            }
            if (this->is_chanop(target_name)) {
                client->reply(ERR_ALREADYOPER, channel_name + SPACE + ":User is already channel operator");
                break ;
            }
            this->add_chanop(target);
            message = "+o " + target_name;
            break;
        case 'k':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, channel_name + SPACE + ":Not enough parameters");
                break ;
            }
            _key = params[2];
            _has_key = true;
            message = "+k " + _key;
            break;
        case 'l':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, channel_name + SPACE + ":Not enough parameters");
                break ;
            }
            limit_param = params[2];
            // Check if limit_param is a number
            if (limit_param.find_first_not_of("0123456789") != std::string::npos) {
                client->reply(ERR_UNKNOWNMODE, channel_name + SPACE + ":Unknown mode char");
                break ;
            }
            // Check if limit is greater than 0
            limit = std::atoi(limit_param.c_str());
            if (limit < 1) {
                client->reply(ERR_UNKNOWNMODE, channel_name + SPACE + ":Unknown mode char");
                break ;
            }
            this->_has_user_limit = true;
            this->_user_limit = limit;
            message = "+l " + limit_param;
            break;
        default:
            client->reply(ERR_UNKNOWNMODE, channel_name + SPACE + ":Unknown mode char");
            break;
    }

    if (message.empty())
        return ;
    this->mode(client, message);
    return ;
}

void     Channel::unset_mode(char mode, std::vector<std::string> params, Client *client, std::string channel_name) {
    std::string message = "";
    Client      *target;
    std::string target_name;
    std::string limit_param;

    switch (mode) {
        case 't':
            _topic_restriction = false;
            message = "-t";
            break;
        case 'i':
            _invite_only = false;
            message = "-i";
            break;
        case 'o':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, channel_name + SPACE + ":Not enough parameters");
                break ;
            }
            target_name = params[2];
            target = get_client(target_name, _clients);
            if (!this->has_client(target)) {
                client->reply(ERR_USERNOTINCHANNEL, channel_name + SPACE + ":User is not on this channel");
                break ;
            }
            if (!this->is_chanop(target_name)) {
                client->reply(ERR_CHANOPRIVSNEEDED, channel_name + SPACE + ":User is not channel operator");
                break ;
            }
            this->remove_chanop(target);
            message = "-o " + target_name;
            break;
        case 'k':
            _key = "";
            _has_key = false;
            message = "-k";
            break;
        case 'l':
            this->_has_user_limit = false;
            this->_user_limit = 0;
            message = "-l";
            break;
        default:
            client->reply(ERR_UNKNOWNMODE, channel_name + SPACE + ":Unknown mode char");
            break;
    }

    if (message.empty())
        return ;
    this->mode(client, message);
    return ;
}

/******************************************************************************/
/*                   Member Functions: Message management                     */
/******************************************************************************/

void    Channel::broadcast(Client *sender, std::string command, std::string target, std::string message) {
    if (target.empty())
        target = this->get_name();
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (command == "PRIVMSG" && *it == sender)
            continue ;
        (*it)->broadcast(sender, command, target, message);
    }
    return ;
}

/******************************************************************************/
/*                                 Getters                                    */
/******************************************************************************/

std::string             Channel::get_name(void) const {
    return this->_name + ":" + this->_hostname;
}

std::string             Channel::get_topic(void) const {
    return this->_topic.empty() ? this->_topic : ":" + this->_topic;
}

std::string             Channel::get_key(void) const {
    return this->_key;
}

std::string             Channel::get_modes(void) {
    std::string modes = "+";
    std::string params = "";
    if (this->_topic_restriction)
        modes += "t";
    if (this->_invite_only)
        modes += "i";
    if (this->_has_key) {
        modes += "k";
        params += this->_key;
    }
    if (this->_op_clients.size() > 0) {
        modes += "o";
        if (!params.empty())
            params += " ";
        params += this->get_chanop_names();
    }
    if (this->has_user_limit()) {
        modes += "l";
        if (!params.empty())
            params += " ";
        std::stringstream ss;
        ss << this->_user_limit;
        params += ss.str();
    }

    // Concatenate modes and params
    std::string response;
    if (params.empty())
        response = modes;
    else
        response = modes + SPACE + params;

    return response;
}

int                     Channel::get_user_limit(void) const {
    return this->_user_limit;
}


int                     Channel::get_user_quantity(void) const {
    return this->_user_quantity;
}

Client *Channel::get_client(std::string nickname, std::vector<Client *> &clients) {
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
        if ((*it)->get_nickname() == nickname) 
            return *it;
    return NULL;
}

std::vector<Client *>   Channel::get_clients(void) {
    return this->_clients;
}

std::vector<Client *>   Channel::get_chanop_clients(void) {
    return this->_op_clients;
}

std::string             Channel::get_client_names(void) {
    std::string names;
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        names += is_chanop((*it)->get_nickname()) ? "@" : "";
        names += (*it)->get_nickname() + " ";
    }
    return names;
}

std::string             Channel::get_chanop_names(void) {
    std::string names;
    for (std::vector<Client *>::const_iterator it = this->_op_clients.begin(); it != this->_op_clients.end(); it++) 
        names += (*it)->get_nickname() + " ";
    return names;
}

std::string             Channel::get_invited_names(void) {
    std::string names;
    for (std::vector<Client *>::const_iterator it = this->_invited_clients.begin(); it != this->_invited_clients.end(); it++) {
        names += (*it)->get_nickname() + " ";
    }
    return names;
}

std::string             Channel::get_client_info(Client *client, Channel *channel) {
    std::string info = "";
    info += client->get_nickname();
    info += SPACE + client->get_username();
    info += SPACE + client->get_hostname();
    info += SPACE + client->get_hostname();
    info += SPACE + client->get_nickname();
    info += " H";
    if (channel->is_chanop(client->get_nickname()))
        info += "@";
    info += " :" + client->get_realname();
    return info;
}

bool                    Channel::get_topic_restriction(void) const {
    return this->_topic_restriction;
}

bool                    Channel::get_invite_only(void) const {
    return this->_invite_only;
}

bool    Channel::has_bot(void) const {
    return this->_has_bot;
}

bool    Channel::has_key(void) const {
    return this->_has_key;
}

bool    Channel::has_user_limit(void) const {
    return this->_has_user_limit;
}

bool    Channel::has_client(Client *client) {
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
        if ((*it)->get_nickname() == client->get_nickname())
            return true;
    return false;
}

/******************************************************************************/
/*                                 Setters                                    */
/******************************************************************************/

void                Channel::set_topic(const std::string topic) {
    this->_topic = topic;
    return ;
}

void                Channel::set_bot(bool has_bot) {
    this->_has_bot = has_bot;
    return ;
}
