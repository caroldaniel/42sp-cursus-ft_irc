/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 22:04:36 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/11 20:35:10 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <map>
# include <vector>
# include <string>
# include <stdexcept>
# include <algorithm>
# include <cctype>
# include "../utils/utils.hpp"

class Message {
private:
    // Attributes
    std::string                 _prefix;
    std::string                 _command;
    std::vector<std::string>    _params;

    void    _tokenize(std::string message);
public:
    // Constructors
    Message(std::string message);
    Message(const Message &other);
    // Destructor
    ~Message(void);
    // Assignment operator
    Message &operator=(const Message &other);

    // Getters
    std::string                 get_prefix(void) const;
    std::string                 get_command(void) const;
    std::vector<std::string>    get_params(void) const;
};

#endif