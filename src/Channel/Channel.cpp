/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 09:47:16 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 21:36:27 by cado-car         ###   ########.fr       */
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
    this->_clients.push_back(client);
    this->_user_quantity++;
    return ;
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

void    Channel::broadcast(Client *sender, std::string message) {
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (*it != sender) {
            std::cout << "Broadcasting to " << (*it)->get_nickname() << std::endl;
            (*it)->broadcast(sender, _name, message);
        }
    }
    return ;
}

void    Channel::kick(Client *client, Client *target, std::string reason) {
    if (client->is_oper() || client == target || this->get_chanop_names().find(client->get_nickname()) != std::string::npos) {
        this->broadcast(client, client->get_nickname() + " KICK " + _name + " " + target->get_nickname() + " :" + reason + "\r\n");
        this->leave(target);
    }
    return ;
}

bool Channel::set_mode(Message *message) {
    std::string target;
    std::string mode;
    if(message->get_params().size() == 2) {
        target = message->get_params()[0];
        mode = message->get_params()[1];
    }

    if(message->get_params().size() == 3) {
        target = message->get_params()[2];
        mode = message->get_params()[1];
    }

    Client *client = this->get_client_by_nickname(target, this->_clients);

    if (mode[0] == '+') {
        for (size_t i = 1; i < mode.length(); i++) {
            switch (mode[i]) {
                case 't':
                    this->_topic_restriction = true;
                    break;
                case 'i':
                    this->_invite_only = true;
                    break;
                case 'o':
                    if (client != NULL) {
                        this->_op_clients.push_back(client);
                        break;
                    } else
                        return false;
                case 'k':
                    if(message->get_params().size() == 3 && client == NULL){
                        this->_key = target;
                        this->_has_key = true;
                        break;
                    }
                    else
                        return false;
                case 'l':
                    if(message->get_params().size() == 3 && client == NULL){
                        this->_user_limit = std::atoi(target.c_str());
                        this->_has_user_limit = true;
                        break;
                    }
                    else
                        return false;
                default:
                    return false;
            }
        }
    } else if (mode[0] == '-') {
        for (size_t i = 1; i < mode.length(); i++) {
            switch (mode[i]) {
                case 't':
                    this->_topic_restriction = false;
                    break;
                case 'i':
                    this->_invite_only = false;
                    break;
                case 'o':
                    if (client != NULL) {
                        std::vector<Client *>::iterator it = std::find(this->_op_clients.begin(), this->_op_clients.end(), client); 
                        if (it != this->_op_clients.end()) {
                            this->_op_clients.erase(it);
                        }
                        break;
                    } else
                        return false;
                case 'k':
                    this->_key = "";
                    this->_has_key = false;
                    break;
                case 'l':
                    this->_user_limit = 0;
                    this->_has_user_limit = false;
                    break;
                default:
                    return false;
            }
        }
    }
    else
        return false;
    return true;
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

std::string             Channel::get_clients_names(void) const {
    std::string names;
    for (std::vector<Client *>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); it++) {
        names += (*it)->get_nickname() + " ";
    }
    return names;
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

void                    Channel::set_key(const std::string key) {
    this->_key = key;
    return ;
}
