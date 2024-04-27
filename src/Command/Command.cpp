/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 21:11:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/27 18:58:24 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/******************************************************************************/
/*                      Constructors and Destructor                           */
/******************************************************************************/

Command::Command(std::string name, Server *server) : _name(name), _server(server) {
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
    this->_name = other._name;
    this->_server = other._server;
    return *this;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

bool        Command::check_bot_command(std::string message, Channel *channel) {
    if (!channel->has_bot()) 
        return false;
    if (message[0] != '!' && message.size() > 1) 
        return false;
    return true;
}

void        Command::bot_reply(Client *client, Channel *channel, std::string message) {
    std::string target_name = channel ? channel->get_name() : client->get_nickname();
    Client      *marvin_bot = _server->get_client("marvin_bot");

    if (marvin_bot) {
        if (message[0] == '!' && message.size() > 1) 
            message = message.substr(1);
        std::vector<std::string> command = split(message);
        
        // check if only one command is given
        if (command.size() > 1) {
            marvin_bot->broadcast(marvin_bot, "PRIVMSG", target_name, "Too many commands!");
            return ;
        }
        // get command all in lowercase
        std::string cmd = command[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
        
        // Check if conversation is in a channel or private
        if (channel) {
            // check if command is valid
            if (cmd == "commands")
                channel->broadcast(marvin_bot, "PRIVMSG", target_name, "Available commands: time, date, joke, leave");
            else if (cmd == "time")
                channel->broadcast(marvin_bot, "PRIVMSG", target_name, "The time is now: " + get_current_time());
            else if (cmd == "date")
                channel->broadcast(marvin_bot, "PRIVMSG", target_name, "Today is: " + get_current_date());
            else if (cmd == "joke")
                channel->broadcast(marvin_bot, "PRIVMSG", target_name, get_random_joke());
            else if (cmd == "leave") {
                if (!channel->is_chanop(client->get_nickname()))
                    channel->broadcast(marvin_bot, "PRIVMSG", target_name, "You don't own me!");
                else {
                    channel->broadcast(marvin_bot, "PRIVMSG", target_name, "Goodbye, cruel world!");
                    channel->set_bot(false);
                    channel->kick(client, marvin_bot, "Kicked");
                }
            }
            else
                channel->broadcast(marvin_bot, "PRIVMSG", target_name, "I'm sorry, I don't understand the command " + cmd + ". Try !commands for a list of available commands.");
        } else {
            // check if command is valid
            if (cmd == "commands")
                client->broadcast(marvin_bot, "PRIVMSG", target_name, "Available commands: time, date, joke, leave");
            else if (cmd == "time")
                client->broadcast(marvin_bot, "PRIVMSG", target_name, "The time is now: " + get_current_time());
            else if (cmd == "date")
                client->broadcast(marvin_bot, "PRIVMSG", target_name, "Today is: " + get_current_date());
            else if (cmd == "joke")
                client->broadcast(marvin_bot, "PRIVMSG", target_name, get_random_joke());
            else if (cmd == "leave")
                client->broadcast(marvin_bot, "PRIVMSG", target_name, "You can't make me leave my own chat! Are you crazy?");
            else
                client->broadcast(marvin_bot, "PRIVMSG", target_name, "I'm sorry, I don't understand the command " + cmd + ". Try !commands for a list of available commands.");
        }                   
    }
    return ;
}

std::string Command::get_current_time(void) {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

std::string Command::get_current_date(void) {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return buf;
}

std::string Command::get_random_joke(void) {
    std::string jokes[] = {
        "Why did the scarecrow win an award? Because he was outstanding in his field!",
        "What do you call a fish wearing a crown? A king fish!",
        "Why don't scientists trust atoms? Because they make up everything!",
        "What do you get when you cross a snowman and a vampire? Frostbite!",
        "Why did the math book look sad? Because it had too many problems.",
        "What do you call a bear with no teeth? A gummy bear!",
        "Why did the tomato turn red? Because it saw the salad dressing!",
        "What do you call a pile of cats? A meowtain!",
        "Why did the golfer bring two pairs of pants? In case he got a hole in one!",
        "What do you call a fake noodle? An impasta!"
    };
    return jokes[rand() % 10];
}
