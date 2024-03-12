/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 09:47:13 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 15:15:53 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "../Client/Client.hpp"

class Channel {
private:
    std::string             _name;
    std::string             _topic;
    std::vector<Client *>   _clients;

public:
    Channel(std::string name);
    Channel(const Channel &other);
    ~Channel(void);
    Channel &operator=(const Channel &other);

    void    join(Client *client);
    void    leave(Client *client);
    void    broadcast(Client *sender, std::string message);

    // Getters
    std::string             get_name(void);
    std::string             get_topic(void);
    std::vector<Client *>   get_clients(void);
    std::string             get_clients_names(void);
};

#endif