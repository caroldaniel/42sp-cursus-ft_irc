/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 20:59:46 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/10 23:34:20 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <vector>
# include <string>

# include "../Server/Server.hpp"

/*
** The Base Command class
*/

class Command {
protected:
    Server *                    _server;
    std::vector<std::string>    _tokens;

public:
    Command(Server &server, std::vector<std::string> tokens);
    Command(const Command &other);
    ~Command(void);

    // Operator overloads
    Command &operator=(const Command &other);

    // Member functions
    virtual void    execute_command(Client *client) = 0;
};

/*
**  The Command classes
*/
class Pass : public Command {
public:
    Pass(Server &server, std::vector<std::string> tokens);
    ~Pass(void);

    // Member functions
    void    execute_command(Client *client);
};

class User : public Command {
public:
    User(Server &server, std::vector<std::string> tokens);
    ~User(void);

    // Member functions
    void    execute_command(Client *client);
};

class Nick : public Command {
public:
    Nick(Server &server, std::vector<std::string> tokens);
    ~Nick(void);

    // Member functions
    void    execute_command(Client *client);
};

class Quit : public Command {
public:
    Quit(Server &server, std::vector<std::string> tokens);
    ~Quit(void);

    // Member functions
    void    execute_command(Client *client);
};

#endif