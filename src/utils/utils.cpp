/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:28:58 by user42            #+#    #+#             */
/*   Updated: 2024/03/11 22:09:52 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void addPollfd(std::vector<pollfd>& fds, int fd, short events) {
    pollfd p;
    p.fd = fd;
    p.events = events;
    p.revents = 0;
    fds.push_back(p);
}

// Split a string into a vector of strings using any whitespace as delimiter.
// No trailing whispace is included in the individual strings.

std::vector<std::string> split(std::string message) {
    std::vector<std::string> tokens;
    std::string currentToken;

    bool inToken = false;
    for (size_t i = 0; i < message.size(); ++i) {
        if (message[i] != ' ' && message[i] != '\t' && message[i] != '\n' && message[i] != '\r') {
            // Character is not whitespace, add it to the current token
            currentToken += message[i];
            inToken = true;
        } else if (inToken) {
            // We are transitioning from a non-whitespace character to whitespace
            // Push back the current token and reset it
            tokens.push_back(currentToken);
            currentToken.clear();
            inToken = false;
        }
    }

    // If the last character was part of a token, add it to the result
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    return tokens;
}
    