/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:04:27 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/10 23:29:24 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Parser::Parser(Server *server, std::string message) : _server(server) {
    _parse_message(message);

    _commands["PASS"] = new Pass(server, _tokens);
    _commands["USER"] = new User(server, _tokens);
    _commands["NICK"] = new Nick(server, _tokens);
    _commands["QUIT"] = new Quit(server, _tokens);
    return ;
}

Parser::Parser(const Parser &other) {
    *this = other;
    return ;
}

Parser::~Parser(void) {
    return ;    
}

/******************************************************************************/
/*                         Operator Overloads                                 */
/******************************************************************************/
Parser &Parser::operator=(const Parser &other) {
    *this = other;
    return *this;
}

/******************************************************************************/
/*                         Private member functions                           */
/******************************************************************************/

void    Parser::_parse_message(std::string message) {
    std::string token;
    std::istringstream iss(message);
    while (std::getline(iss, token, ' ')) {
        _tokens.push_back(token);
    }
    return ;
}

/******************************************************************************/
/*                             Member functions                               */
/******************************************************************************/

void    Parser::invoke_command(Client *client) {
    std::string command = _tokens[0];
    if (_commands.find(command) != _commands.end()) {
        _commands[command]->execute_command(client);
    }
    // Check if client was disconnected
    if (client->get_socket() == -1) {
        _server->on_client_disconnect(client->get_socket());
    }
    return ;
}
