/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCmacros.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:06:24 by cado-car          #+#    #+#             */
/*   Updated: 2024/03/12 21:00:13 by cado-car         ###   ########.fr       */
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
**  Connection Registration
*/
# define RPL_WELCOME                "001" // :Welcome to the Internet Relay Network <nick>!<user>@<host
# define RPL_YOURHOST               "002" // :Your host is <servername>, running version <ver>
# define RPL_CREATED                "003" // :This server was created <date>
# define RPL_MYINFO                 "004" // <servername> <version> <available user modes> <available channel modes>

/*
**  Quit Command
*/
# define RPL_QUIT                   "101" // :Goodbye!

/*
**  List Command
*/
# define RPL_LISTSTART              "321" // Channel :Users  Name
# define RPL_LIST                   "322" // <channel> <# visible> :<topic>
# define RPL_LISTEND                "323" // :End of /LIST

/*
**  Join Command
*/
# define RPL_TOPIC                  "332" // <channel> :<topic>
# define RPL_NOTOPIC                "331" // <client> <channel> :No topic is set
# define RPL_NAMREPLY               "353" // = <channel> :<nick1> <nick2> <nick3> ...
# define RPL_ENDOFNAMES             "366" // <channel> :End of /NAMES list
# define RPL_YOUREOPER              "381" // <client> :You are now an IRC operator

/*
**  Mode Command
*/
# define RPL_CHANNELMODEIS          "324" // <channel> <mode> <mode params>

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
# define ERR_UNKNOWNCOMMAND         "421" // <command> :Unknown command
# define ERR_NONICKNAMEGIVEN        "431" // :No nickname given
# define ERR_NICKNAMEINUSE          "433" // <nick> :Nickname is already in use
# define ERR_NOTONCHANNEL           "442" // <client> <channel> :You're not on that channel
# define ERR_NEEDMOREPARAMS         "461" // :Not enough parameters
# define ERR_ALREADYREGISTRED       "462" // :You may not reregister
# define ERR_PASSWDMISMATCH         "464" // :Password incorrect
# define ERR_NOPRIVILEGES           "481" // :Permission Denied 
# define ERR_CHANOPRIVSNEEDED       "482" // <client> <channel> :You're not channel operator 
# define ERR_USERNOTINCHANNEL       "441" // <nick> <channel> :They aren't on that channel

#endif