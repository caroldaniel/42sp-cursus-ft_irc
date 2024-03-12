/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 20:59:46 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/11 22:34:51 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <vector>
# include <string>

# include "../Client/Client.hpp"
# include "../Message/Message.hpp"

/******************************************************************************/
/*                            Base Class Definition                           */
/******************************************************************************/

class Command {
public:
    // Constructors
    Command(void);
    Command(const Command &other);
    // Virtual Destructor
    virtual ~Command(void);
    // Assignment operator
    Command &operator=(const Command &other);

    // Member functions
    virtual void    invoke(Client *client, Message *message) = 0;
};


/******************************************************************************/
/*                            Derived Classes                                 */
/******************************************************************************/

class Pass : public Command {
public:
    Pass(void);
    ~Pass(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class User : public Command {
public:
    User(void);
    ~User(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Nick : public Command {
public:
    Nick(void);
    ~Nick(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Quit : public Command {
public:
    Quit(void);
    ~Quit(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

#endif