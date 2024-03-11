/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 21:11:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/10 22:29:51 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Command::Command(Server *server, std::vector<std::string> tokens) : _server(server), _tokens(tokens) {
    return ;
}

Command::Command(const Command &other) {
    *this = other;
    return ;
}

Command::~Command(void) {
    return ;    
}

/******************************************************************************/
/*                         Operator Overloads                                 */
/******************************************************************************/

Command &Command::operator=(const Command &other) {
    *this = other;
    return *this;
}

