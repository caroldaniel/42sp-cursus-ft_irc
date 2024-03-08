/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dofranci <dofranci@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-03-08 21:07:20 by dofranci          #+#    #+#             */
/*   Updated: 2024-03-08 21:07:20 by dofranci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "../Client/Client.hpp"
# include "../Server/Server.hpp"

class Parser
{
    public:
        static void handleClientMessage(int fd, std::map<int, Client *> _clients, std::string message);
        static void setupClientData(int fd, std::map<int, Client *> _clients, std::string message);
};

#endif