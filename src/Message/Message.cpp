/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:04:27 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/11 20:34:17 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/
Message::Message(std::string message) {
    try {
        _tokenize(message);        
    }
    catch (std::exception &e) {
        throw std::runtime_error("Parse failed: " + std::string(e.what()));
    }
    return ;
}

Message::Message(const Message &other) {
    *this = other;
    return ;
}

Message::~Message(void) {
    return ;
}

/******************************************************************************/
/*                         Operator Overloads                                 */
/******************************************************************************/

Message &Message::operator=(const Message &other) {
    *this = other;
    return *this;
}

/******************************************************************************/
/*                         Private Member functions                           */
/******************************************************************************/

void    Message::_tokenize(std::string message) {
    std::vector<std::string> split_msg = split(message);

    if (split_msg.size() < 1) {
        throw std::runtime_error("Message is empty");
    }

    // Check if first token is a prefix and take it out of the vector
    if (split_msg[0][0] == ':') {
        _prefix = split_msg[0].substr(1);
        split_msg.erase(split_msg.begin());
    }

    // The next token is the command
    _command = split_msg[0];

    // The rest of the tokens are parameters
    for (size_t i = 1; i < split_msg.size(); i++) {
        //if the token starts with a colon, everything after it is a single parameter
        if (split_msg[i][0] == ':') {
            std::string param = split_msg[i].substr(1);
            for (size_t j = i + 1; j < split_msg.size(); j++) {
                param += " " + split_msg[j];
            }
            _params.push_back(param);
            break;
        }
        _params.push_back(split_msg[i]);
    }
    return ;
}

/******************************************************************************/
/*                                 Getters                                    */
/******************************************************************************/

std::string                 Message::get_prefix(void) const {
    return _prefix;
}

std::string                 Message::get_command(void) const {
    return _command;
}

std::vector<std::string>    Message::get_params(void) const {
    return _params;
}

