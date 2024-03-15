/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:03:43 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 20:58:58 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

/*
** Libraries
*/
# include "../Client/Client.hpp"
# include "../Channel/Channel.hpp"
# include "../Message/Message.hpp"

class Command;

/*
** Server class
*/

class Server {
private:

    // Attributes
    bool                                _running;
    int                                 _socket;
    
    const std::string                   _port;
    const std::string                   _password;
    const std::string                   _hostname;

    std::vector<pollfd>                 _pollfds;
    std::map<int, Client *>             _clients;
    std::vector<Channel *>              _channels;

    std::map<std::string, Command *>    _commands;

public:

    // Constructors
    Server(std::string port, std::string password);
    Server(const Server &other);
    // Destructor
    ~Server();
    // Assignment operator
    Server  &operator=(const Server &other);

    // Getters
    Client  *get_client(int client_fd);
    Client  *get_client_by_nickname(std::string nickname);
    Channel *get_channel(std::string name);
    std::vector<Channel *> list_channels(void);

    // Member functions on Client's actions
    void    on_client_connect(void);
    void    on_client_disconnect(int client_fd);
    void    on_client_message(int client_fd, std::string message);

    // Member functions on Server's actions
    void    create_socket(void);
    void    start(void);

    void    add_channel(Channel *channel);
};

/******************************************************************************/
/*                         Command Class Definition                           */
/******************************************************************************/

class Command {
protected:
    std::string _name;
    Server      *_server;
    
public:
    // Constructors
    Command(std::string name, Server *server);
    Command(const Command &other);
    // Virtual Destructor
    virtual ~Command(void);
    // Assignment operator
    Command &operator=(const Command &other);

    // Member functions
    virtual void    invoke(Client *client, Message *message) = 0;
};


/******************************************************************************/
/*                      Derived Classes from Command                          */
/******************************************************************************/

class Pass : public Command {
public:
    Pass(Server *server);
    ~Pass(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class User : public Command {
public:
    User(Server *server);
    ~User(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Nick : public Command {
public:
    Nick(Server *server);
    ~Nick(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Quit : public Command {
public:
    Quit(Server *server);
    ~Quit(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class List : public Command {
public:
    List(Server *server);
    ~List(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Join : public Command {
public:
    Join(Server *server);
    ~Join(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Privmsg : public Command {
public:
    Privmsg(Server *server);
    ~Privmsg(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Oper : public Command {
public:
    Oper(Server *server);
    ~Oper(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Topic : public Command {
public:
    Topic(Server *server);
    ~Topic(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Mode : public Command {
public:
    Mode(Server *server);
    ~Mode(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

class Part : public Command {
public:
    Part(Server *server);
    ~Part(void);

    // Member functions
    void    invoke(Client *client, Message *message);
};

#endif