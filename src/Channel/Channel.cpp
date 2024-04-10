/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 09:47:16 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/07 19:45:56 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Channel::Channel(std::string name) : _name(name), _topic("No topic"), _key(""), _user_limit(0), _user_quantity(0),_topic_restriction(true), _invite_only(false), _has_key(false), _has_user_limit(false) {
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
    if (get_client_by_nickname(client->get_nickname(), _clients) == NULL) {
        _clients.push_back(client);
        _user_quantity++;
    }
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

void    Channel::invite(Client *client) {
    this->_invited_clients.push_back(client);
    return ;
}   

void    Channel::leave(Client *client) {
    std::vector<Client *>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), client); 
    if (it != this->_clients.end()) {
        this->_clients.erase(it);
    }
    this->_user_quantity--;
    return ;
}

void    Channel::kick(Client *client, Client *target, std::string reason) {
    if (client == target || client->is_oper() || is_chanop(client->get_nickname())) {
        broadcast(client, client->get_nickname() + " KICK " + _name + " " + target->get_nickname() + " :" + reason + "\r\n");
        leave(target);
    }
    return ;
}

void    Channel::broadcast(Client *sender, std::string message) {
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (*it != sender) {
            std::cout << "Broadcasting to " << (*it)->get_nickname() << std::endl;
            (*it)->broadcast(sender, _name, message);
        }
    }
    return ;
}

void     Channel::set_mode(char mode, std::vector<std::string> params, Client *client) {
    
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
                client->reply(ERR_NEEDMOREPARAMS, "MODE", ":Not enough parameters");
                break ;
            }
            if (client == NULL) {
                client->reply(ERR_NOSUCHNICK, params[2], ":No such nick/channel");
                break ;
            }
            if (get_client_by_nickname(params[2], _clients) == NULL) {
                client->reply(ERR_USERNOTINCHANNEL, params[2], ":User is not on that channel");
                break ;
            }
            if (get_chanop_names().find(params[2]) == std::string::npos)
                add_chanop(get_client_by_nickname(params[2], _clients));            
            message = "+o :Added " + params[2] + " as channel operator";
            break;
        case 'k':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, "MODE", ":Not enough parameters");
                break ;
            }
            _key = params[2];
            _has_key = true;
            message = "+k :Channel now is keyed. Key is: " + params[2];
            break;
        case 'l':
            if (params.size() < 3) {
                client->reply(ERR_NEEDMOREPARAMS, "MODE", ":Not enough parameters");
                break ;
            }
            this->_user_limit = std::atoi(params[2].c_str());
            this->_has_user_limit = true;
            message = "+l :Channel now has user limit of " + params[2];
            break;
        default:
            client->reply(ERR_UNKNOWNMODE, params[1], ":Unknown mode char");
            break;
    }
    std::string response = "MODE " + _name + " " + message;
    broadcast(client, response);
}

void     Channel::unset_mode(char mode, std::vector<std::string> params, Client *client) {
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
                client->reply(ERR_NEEDMOREPARAMS, "MODE", ":Not enough parameters");
                break ;
            }
            if (client == NULL) {
                client->reply(ERR_NOSUCHNICK, params[2], ":No such nick/channel");
                break ;
            }
            if (get_client_by_nickname(params[2], _clients) == NULL) {
                client->reply(ERR_USERNOTINCHANNEL, params[2], ":User is not on that channel");
                break ;
            }
            if (get_chanop_names().find(params[2]) == std::string::npos) {
                client->reply(ERR_CHANOPRIVSNEEDED, params[2], ":User is not channel operator");
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
            client->reply(ERR_UNKNOWNMODE, params[1], ":Unknown mode char");
            break;
    }
    std::string response = "MODE " + _name + " " + message;
    this->broadcast(client, response);
    return ;
}

/******************************************************************************/
/*                                 Getters                                    */
/******************************************************************************/

std::string             Channel::get_name(void) const {
    return this->_name;
}

std::string             Channel::get_topic(void) const {
    return this->_topic;
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

int                    Channel::get_user_quantity(void) const {
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

std::string Channel::get_clients_names(void)
{
    std::string names;
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        names += is_chanop((*it)->get_nickname()) ? "@" : "";
        names += (*it)->get_nickname() + " ";
    }
    return names;
}

int                   Channel::get_user_limit(void) const {
    return this->_user_limit;
}

bool                    Channel::get_topic_restriction(void) const {
    return this->_topic_restriction;
}

bool                    Channel::get_invite_only(void) const {
    return this->_invite_only;
}

bool                    Channel::get_has_key(void) const {
    return this->_has_key;
}

bool                    Channel::get_has_user_limit(void) const {
    return this->_has_user_limit;
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

