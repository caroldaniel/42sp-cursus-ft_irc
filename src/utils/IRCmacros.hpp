/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCmacros.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cado-car <cado-car@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:06:24 by cado-car          #+#    #+#             */
/*   Updated: 2024/04/26 18:26:45 by cado-car         ###   ########.fr       */
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
# define RPL_NEWNICK                "005" // :Your new nickname is <nick>

/*
**  Quit Command
*/
# define RPL_QUIT                   "101" // :Goodbye!

/*
**  List Command
*/
# define RPL_CAP                    "302" // <client> <subcommand> :are supported capabilities
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
# define RPL_UMODEIS                "221" // <client> <mode>
# define RPL_ENDOFWHO               "315" // <mask> :End of WHO list
# define RPL_CHANNELMODEIS          "324" // <channel> <mode> <mode params>
# define RPL_WHOREPLY               "352" // <client> <username> <hostname> <servername> <nick> <H|G>[*][@|+] :<hopcount> <realname>
# define RPL_ENDOFMODES             "368" // <client> :End of channel mode list

/*
**  Invite Command
*/
# define RPL_INVITING               "341" // <client> <nick> <channel>

/*
**  Whois Command
*/
# define RPL_WHOISUSER              "311" // <client> <nick> <username> <hostname> * :<realname>
# define RPL_WHOISSERVER            "312" // <client> <nick> <server> :<server info>
# define RPL_WHOISOPERATOR          "313" // <client> <nick> :is an IRC operator
# define RPL_WHOISIDLE              "317" // <client> <nick> <seconds> <signon> :seconds idle, signon time
# define RPL_ENDOFWHOIS             "318" // <client> <nick> :End of WHOIS list
# define RPL_WHOISCHANNELS          "319" // <client> <nick> :<channels>

/*
** PING / PONG Commands
*/
# define RPL_PING                   "PONG" // :<msg>

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
# define ERR_ALREADYONCHANNEL       "443" // <channel> :You're already on that channel
# define ERR_NOTREGISTERED          "451" // :You have not registered
# define ERR_NEEDMOREPARAMS         "461" // :Not enough parameters
# define ERR_ALREADYREGISTRED       "462" // :You may not reregister
# define ERR_PASSWDMISMATCH         "464" // :Password incorrect
# define ERR_UNKNOWNMODE            "472" // :Unknown mode char
# define ERR_BADCHANNELKEY          "475" // <channel> :Cannot join channel (+k)
# define ERR_NOPRIVILEGES           "481" // :Permission Denied 
# define ERR_CHANOPRIVSNEEDED       "482" // <client> <channel> :You're not channel operator 
# define ERR_USERNOTINCHANNEL       "441" // <nick> <channel> :They aren't on that channel
# define ERR_USERONCHANNEL          "443" // <nick> <channel> :is already on channel 
# define ERR_INVITEONLYCHAN         "473" // <channel> :Cannot join channel (+i)
# define ERR_BADCHANNELKEY          "475" // <channel> :Cannot join channel (+k)
# define ERR_CHANNELISFULL          "471" // <channel> :Cannot join channel (+l)
# define ERR_NOTOPER                "481" // <nick> :is not an IRC operator
# define ERR_ERRONEUSNICKNAME       "432" // <client> <nick> :Erroneus nickname 
# define ERR_ERRONEUSUSERNAME       "467" // <client> <username> :Erroneus username
# define ERR_ERRONEUSREALNAME       "472" // <client> <realname> :Erroneus realname
# define ERR_ALREADYOPER            "485" // <client> :You're already an IRC operator

#endif