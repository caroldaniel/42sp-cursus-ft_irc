/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:04:36 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/10 23:37:21 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "../Command/Command.hpp"

class Parser {
private:
    Server                              *_server;
    std::vector<std::string>            _tokens;
    std::map<std::string, Command *>    _commands;
    
    void                                _parse_message(std::string message);

public:
    Parser(Server *server, std::string message);
    Parser(const Parser &other);
    ~Parser(void);

    // Operator overloads
    Parser &operator=(const Parser &other);

    // Member functions
    void    invoke_command(Client *client);
};

#endif