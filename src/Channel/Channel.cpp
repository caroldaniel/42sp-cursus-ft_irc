/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 09:47:16 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/25 22:42:57 by cado-car         ###   ########.fr       */
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
/*                              Member Functions                              */
/******************************************************************************/

void    Channel::join(Client *client) {

    if (this->has_client(client))
        return ;

    // Check if channel has reached user limit
    if (this->has_user_limit() && this->get_user_quantity() >= this->get_user_limit()) {
        client->reply(ERR_CHANNELISFULL, this->get_name() + SPACE + ":Cannot join channel (+l)");
        return ;
    }
    
    this->_clients.push_back(client);
    this->_user_quantity++;
    
    std::string join_message;
    join_message = ":" + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname() + " JOIN " + this->get_name() + CRLF;
    this->send_to_all(join_message);
    return ;
}

void    Channel::part(Client *client, std::string message) {

    if (!message.empty())
        message = ":" + message;

    if (!this->has_client(client))
        return ;
    this->leave(client);
    
    std::string part_message;
    part_message = ":" + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname() + " PART " + this->get_name() + SPACE + message + CRLF;
    this->send_to_all(part_message);
    return ;
}

void    Channel::quit(Client *client, std::string message) {

    if (!message.empty())
        message = ":" + message;
    
    if (!this->has_client(client))
        return ;
    this->leave(client);
    
    std::string quit_message;
    quit_message = ":" + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname() + " QUIT " + message + CRLF;
    this->send_to_all(quit_message);
    return ;
    return ;
}

void    Channel::topic(Client *client, std::vector<std::string> params) {
    
    // Get topic of channel
    if (params.size() == 1) {
        if (this->get_topic().empty()) {
            client->reply(RPL_NOTOPIC, this->get_name() + SPACE + ":No topic is set");
        } else {
            client->reply(RPL_TOPIC, this->get_name() + SPACE + this->get_topic());
        }
    }

    // Set topic of channel
    if (params.size() == 2) {
        std::string topic = params[1];
        this->set_topic(topic);

        std::string topic_message;
        topic_message = ":" + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname() + " TOPIC " + this->get_name() + SPACE + this->get_topic() + CRLF;
        this->send_to_all(topic_message);
    }
    return ;
}

void    Channel::names(Client *client) {
    client->reply(RPL_NAMREPLY, "= " + this->get_name() + " :" + this->get_clients_names());
    client->reply(RPL_ENDOFNAMES, this->get_name() + " :End of /NAMES list");
    return ;
}

void    Channel::mode(Client *client, std::string message) {
    std::string mode_message;
    mode_message = ":" + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname() + " MODE " + message + CRLF;
    this->send_to_all(mode_message);
}


void    Channel::kick(Client *client, Client *target, std::string reason) {

    if (client != target && !is_chanop(client->get_nickname())) {
        client->reply(ERR_CHANOPRIVSNEEDED, this->get_name() + SPACE + ":You're not channel operator");
        return ;    
    }
    std::string kick_message;
    kick_message = ":" + client->get_nickname() + "!" + client->get_username() + "@" + client->get_hostname() + " KICK " + this->get_name() + SPACE + target->get_nickname() + " :" + reason + CRLF;
    this->send_to_all(kick_message);
    this->leave(target);
    return ;
}

void    Channel::add_chanop(Client *client) {
    this->_op_clients.push_back(client);
    return ;
}

void    Channel::remove_chanop(Client *client) {
    std::vector<Client *>::iterator it = std::find(this->_op_clients.begin(), this->_op_clients.end(), client); 
    if (it != this->_op_clients.end()) {
        this->_op_clients.erase(it);
    }
    return ;
}

bool    Channel::is_chanop(std::string nickname) {
    for (std::vector<Client *>::iterator it = this->_op_clients.begin(); it != this->_op_clients.end(); it++) {
        if ((*it)->get_nickname() == nickname) {
            return true;
        }
    }
    return false;
}

bool    Channel::has_client(Client *client) {
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if ((*it)->get_nickname() == client->get_nickname()) {
            return true;
        }
    }
    return false;
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



void    Channel::invite(Client *client) {
    this->_invited_clients.push_back(client);
    return ;
}   

void    Channel::leave(Client *client) {
    // Remove client from channel operators
    std::vector<Client *>::iterator it_op = std::find(this->_op_clients.begin(), this->_op_clients.end(), client);
    if (it_op != this->_op_clients.end()) {
        this->_op_clients.erase(it_op);
    }
    // Remove client from channel
    std::vector<Client *>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), client); 
    if (it != this->_clients.end()) {
        this->_clients.erase(it);
        this->_user_quantity--;
    }
    return ;
}


void    Channel::send_to_all(std::string message) {
    std::cout << "Sending to all in " << get_name() << ": " << message << std::endl;
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
        send((*it)->get_socket(), message.c_str(), message.size(), 0);
    return ;
}

void    Channel::broadcast(Client *sender, std::string message) {
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (*it != sender) {
            (*it)->broadcast(sender, _name, message);
        }
    }
    return ;
}

void     Channel::set_mode(char mode, std::vector<std::string> params, Client *client, std::string channel_name) {
    
    std::string message = "";

    switch (mode) {
        case 't':
            _topic_restriction = true;
            message = "+t :Topic restriction is enabled";
            break;
        case 'i':
            _invite_only = true;
            message = "+i :Invite only is enabled";
            break;
        case 'o':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, channel_name + SPACE + ":Not enough parameters");
                break ;
            }
            if (client == NULL) {
                client->reply(ERR_NOSUCHNICK, channel_name + SPACE + ":No such nick/channel");
                break ;
            }
            if (get_client_by_nickname(params[2], _clients) == NULL) {
                client->reply(ERR_USERNOTINCHANNEL, channel_name + SPACE + ":User is not on this channel");
                break ;
            }
            if (get_chanop_names().find(params[2]) == std::string::npos)
                add_chanop(get_client_by_nickname(params[2], _clients));            
            message = "+o :Added " + params[2] + " as channel operator";
            break;
        case 'k':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, channel_name + SPACE + ":Not enough parameters");
                break ;
            }
            _key = params[2];
            _has_key = true;
            message = "+k :Channel now is keyed. Key is: " + params[2];
            break;
        case 'l':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, channel_name + SPACE + ":Not enough parameters");
                break ;
            }
            this->_user_limit = std::atoi(params[2].c_str());
            this->_has_user_limit = true;
            message = "+l :Channel now has user limit of " + params[2];
            break;
        default:
            client->reply(ERR_UNKNOWNMODE, channel_name + SPACE + ":Unknown mode char");
            break;
    }

    if (message.empty())
        return ;
    std::string response = this->_name + " " + message;
    this->mode(client, response);
    return ;
}

void     Channel::unset_mode(char mode, std::vector<std::string> params, Client *client, std::string channel_name) {
    std::string message = "";

    switch (mode) {
        case 't':
            _topic_restriction = false;
            message = "-t :Topic restriction is disabled";
            break;
        case 'i':
            _invite_only = false;
            message = "-i :Invite only is disabled";
            break;
        case 'o':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, channel_name + SPACE + ":Not enough parameters");
                break ;
            }
            if (client == NULL) {
                client->reply(ERR_NOSUCHNICK, channel_name + SPACE + ":No such nick/channel");
                break ;
            }
            if (get_client_by_nickname(params[2], _clients) == NULL) {
                client->reply(ERR_USERNOTINCHANNEL, channel_name + SPACE + ":User is not on this channel");
                break ;
            }
            if (get_chanop_names().find(params[2]) == std::string::npos) {
                client->reply(ERR_CHANOPRIVSNEEDED, channel_name + SPACE + ":User is not channel operator");
                break ;
            }
            remove_chanop(get_client_by_nickname(params[2], _clients));
            message = "-o :Removed " + params[2] + " as channel operator";
            break;
        case 'k':
            _key = "";
            _has_key = false;
            message = "-k :Channel now is not keyed";
            break;
        case 'l':
            _user_limit = 0;
            _has_user_limit = false;
            message = "-l :Channel now has no user limit";
            break;
        default:
            client->reply(ERR_UNKNOWNMODE, channel_name + SPACE + ":Unknown mode char");
            break;
    }

    if (message.empty())
        return ;
    std::string response = this->_name + " " + message;
    this->mode(client, response);
    return ;
}

void     Channel::update_list_names(void) {
    std::vector<Client *> clients = get_clients();

    // Loop through clients and call names() on each one
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        this->names(*it);
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
    if (this->_topic.empty())
        return this->_topic;
    return ":" + this->_topic;
}

std::string            Channel::get_key(void) const {
    return this->_key;
}

std::string             Channel::get_chanop_names(void) const {
    std::string names;
    for (std::vector<Client *>::const_iterator it = this->_op_clients.begin(); it != this->_op_clients.end(); it++) {
        names += (*it)->get_nickname() + " ";
    }
    return names;
}

std::string             Channel::get_invited_names(void) const {
    std::string names;
    for (std::vector<Client *>::const_iterator it = this->_invited_clients.begin(); it != this->_invited_clients.end(); it++) {
        names += (*it)->get_nickname() + " ";
    }
    return names;
}

std::vector<Client *>   Channel::get_clients(void) const {
    return this->_clients;
}

std::vector<Client *>   Channel::get_chanop_clients(void) const {
    return this->_op_clients;
}

int                     Channel::get_user_quantity(void) const {
    return this->_user_quantity;
}

std::string             Channel::get_modes(void) {
    std::string modes = "+";
    if (this->_topic_restriction)
        modes += "t";
    if (this->_invite_only)
        modes += "i";
    if (this->_key != "")
        modes += "k";
    if (this->_op_clients.size() > 0)
        modes += "o";

    // if modes is empty, return "No modes set"
    if (modes == "+")
        return ":No modes set";
    return modes;
}

std::string             Channel::get_clients_names(void)
{
    std::string names;
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        names += is_chanop((*it)->get_nickname()) ? "@" : "";
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

int                     Channel::get_user_limit(void) const {
    return this->_user_limit;
}

bool                    Channel::get_topic_restriction(void) const {
    return this->_topic_restriction;
}

bool                    Channel::get_invite_only(void) const {
    return this->_invite_only;
}

Client *Channel::get_client_by_nickname(std::string nickname, std::vector<Client *> &clients) {
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->get_nickname() == nickname) {
            return *it;
        }
    }
    return NULL;
}

/******************************************************************************/
/*                                 Setters                                    */
/******************************************************************************/

void                    Channel::set_topic(const std::string topic) {
    this->_topic = topic;
    return ;
}

void                Channel::set_bot(bool has_bot) {
    this->_has_bot = has_bot;
    return ;
}

