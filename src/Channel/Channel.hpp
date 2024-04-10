/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 09:47:13 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/07 18:46:41 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "../Client/Client.hpp"
# include "../Message/Message.hpp"

class Channel {
private:
    std::string             _name;
    std::string             _topic;
    std::string             _key;
    int                     _user_limit;
    int                     _user_quantity;
    std::vector<Client *>   _clients;
    std::vector<Client *>   _op_clients;
    std::vector<Client *>   _invited_clients;

    bool                    _topic_restriction;
    bool                    _invite_only;
    bool                    _has_key;
    bool                    _has_user_limit;

public:
    Channel(std::string name);
    Channel(const Channel &other);
    ~Channel(void);
    Channel &operator=(const Channel &other);

    void                    join(Client *client);
    void                    add_chanop(Client *client);
    void                    remove_chanop(Client *client);
    bool                    is_chanop(std::string nickname);
    void                    invite(Client *client);
    void                    leave(Client *client);
    void                    kick(Client *client, Client *target, std::string reason);
    void                    broadcast(Client *sender, std::string message);
    void                    set_mode(char mode, std::vector<std::string> params, Client *client);
    void                    unset_mode(char mode, std::vector<std::string> params, Client *client);

    // Getters
    std::string             get_name(void) const;
    std::string             get_modes(void);
    std::string             get_topic(void) const;
    std::vector<Client *>   get_clients(void) const;
    std::string             get_clients_names(void);
    std::string             get_chanop_names(void) const;
    std::string             get_invited_names(void) const;
    std::string             get_key(void) const;
    int                     get_user_quantity(void) const;
    int                     get_user_limit(void) const;
    bool                    get_topic_restriction(void) const;
    bool                    get_invite_only(void) const;
 
    bool                    get_has_key(void) const;
    bool                    get_has_user_limit(void) const;
    Client                  *get_client_by_nickname(std::string nickname, std::vector<Client *> &clients);

    // Setters
    void                    set_topic(const std::string topic);
    void                    set_user_limit(const int user_limit);
    bool                    set_mode(Message *message);
};

#endif