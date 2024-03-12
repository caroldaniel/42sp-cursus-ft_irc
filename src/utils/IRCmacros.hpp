/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCmacros.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:06:24 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 12:03:41 by cado-car         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCMACROS_HPP
# define IRCMACROS_HPP

# define CRLF "\r\n"
# define SPACE " "

/******************************************************************************/
/*                           NUMERICAL REPLIES                                */
/******************************************************************************/

/*
**  001 - 004: Connection Registration
*/
# define RPL_WELCOME                "001" // :Welcome to the Internet Relay Network <nick>!<user>@<host
# define RPL_YOURHOST               "002" // :Your host is <servername>, running version <ver>
# define RPL_CREATED                "003" // :This server was created <date>
# define RPL_MYINFO                 "004" // <servername> <version> <available user modes> <available channel modes>

/*
** 101: Quit Command
*/
# define RPL_QUIT                   "101" // :Goodbye!

/*
**  461 - 462: Command Errors
*/

# define ERR_NEEDMOREPARAMS         "461" // :Not enough parameters
# define ERR_ALREADYREGISTRED       "462" // :You may not reregister

/*
**  Password Error
*/
# define ERR_PASSWDMISMATCH         "464" // :Password incorrect

/*
**  Nickname Command
*/

# define ERR_NONICKNAMEGIVEN        "431" // :No nickname given
# define ERR_NICKNAMEINUSE          "433" // <nick> :Nickname is already in use

/*
**  Unknwon Command
*/
# define ERR_UNKNOWNCOMMAND         "421" // <command> :Unknown command


/******************************************************************************/
/*                             ERROR REPLIES                                  */
/******************************************************************************/

# define ERR_NOSUCHNICK             "401" // <nickname> :No such nick/channel
# define ERR_NOSUCHSERVER           "402" // <server name> :No such server
# define ERR_NOSUCHCHANNEL          "403" // <channel name> :No such channel
# define ERR_CANNOTSENDTOCHAN       "404" // <channel name> :Cannot send to channel
# define ERR_TOOMANYCHANNELS        "405" // <channel name> :You have joined too many channels
# define ERR_WASNOSUCHNICK          "406" // <nickname> :There was no such nickname
# define ERR_TOOMANYTARGETS         "407" // <target> :Duplicate recipients. No message delivered

# define RPL_LISTSTART              "321" // Channel :Users  Name
# define RPL_LIST                   "322" // <channel> <# visible> :<topic>
# define RPL_LISTEND                "323" // :End of /LIST

#endif