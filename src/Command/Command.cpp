/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 21:11:57 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 15:47:56 by cado-car         ###   ########.fr       */
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
    *this = other;
    return *this;
}

/******************************************************************************/
/*                         Member functions                                   */
/******************************************************************************/

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
