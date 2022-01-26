#ifndef FT_IRC_ERROR_REPLY_HPP
#define FT_IRC_ERROR_REPLY_HPP

/*
 *  Error Replies
 */

#define ERR_NOSUCHNICK(nick)						":IRC 401 " + nick + " :No such nick/channel\n"
#define ERR_NOSUCHCHANNEL(nick, channel)			":IRC 403 " + nick + " " + channel + " :No such channel\n"
#define ERR_CANNOTSENDTOCHAN(nick, channel)			":IRC 404 " + nick + " " + channel + " :Cannot send to channel\n"
#define ERR_TOOMANYCHANNELS(nick, channel)			":IRC 405 " + nick + " " + channel + " :You have joined too many channels\n"
#define ERR_NORECIPIENT(nick, command)				":IRC 411 " + nick + " :No recipient given " + command + "\n"
#define ERR_NOTEXTTOSEND(nick)						":IRC 412 " + nick + " :No text to send\n"
#define ERR_NONICKNAMEGIVEN							":IRC 431 :No nickname given\n"
#define ERR_ERRONEUSNICKNAME(nick)					":IRC 432 " + nick + " :Erroneus nickname\n"
#define ERR_NICKNAMEINUSE(nick)						":IRC 433 " + nick + " :Nickname is already in use\n"
#define ERR_NICKCOLLISION(nick)						":IRC 436 " + nick + " :Nickname collision KILL\n"
#define ERR_NOTONCHANNEL(nick, channel)				":IRC 442 " + nick + " " + channel + " :You're not on that channel\n"
#define ERR_NOTREGISTERED							":IRC 451 * :You have not registered\n"
#define ERR_NEEDMOREPARAMS(nick, command)			":IRC 461 " + nick + " " + command + " :Not enough parameters\n"
#define ERR_ALREADYREGISTRED(nick)					":IRC 462 " + nick + " :You may not reregister\n"
#define ERR_PASSWDMISMATCH(nick)					":IRC 464 " + nick + " :Password incorrect\n"
#define ERR_CHANNELISFULL(nick, channel)			":IRC 471 " + nick + " " + channel + " :Cannot join channel (+l)\n"
#define ERR_UNKNOWNMODE(nick, flags)				":IRC 472 " + nick + " " +  flags + " :is unknown mode char to me\n"
#define ERR_BADCHANNELKEY(nick, channel)			":IRC 475 " + nick + " " + channel + " :Cannot join channel (+k)\n"
#define ERR_NOPRIVILEGES(nick)						":IRC 481 " + nick + " :Permission Denied- You're not an IRC operator\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel)			":IRC 482 " + nick + " " + channel + " :You're not channel operator\n"
#define ERR_NOOPERHOST(nick)						":IRC 491 " + nick + " :No O-lines for your host\n"
#define ERR_UMODEUNKNOWNFLAG(nick, channel)			":IRC 501 " + nick + " " + channel + " :Unknown MODE flag\n"
#define ERR_USERSDONTMATCH(nick)					":IRC 502 " + nick + " :Cant change mode for other users\n"

/*
 *  Command responses
 */

#define RPL_WELCOME(nick) 							":IRC 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\n"
# define RPL_UMODEIS(nick, mode) 					":IRC 221 " + nick + " " + mode + "\n"
#define	RPL_AWAY(nick, message)						":IRC 301 * " + nick + " :" + message + "\n"
#define RPL_UNAWAY(nick)    	                   	":IRC 305 * " + nick + " :You are no longer marked as being away\n"
#define RPL_NOWAWAY(nick) 							":IRC 306 * " + nick + " :You have been marked as being away\n"
#define RPL_LISTSTART(nick)							":IRC 321 " + nick + " Channel :Users Name\n"
#define	RPL_LIST(nick, channel, numusers, topic)	":IRC 322 " + nick + " " + channel + " " + numusers + " : " + topic + "\n"
#define RPL_LISTEND(nick)							":IRC 323 " + nick + " :End of /LIST\n"
#define	RPL_NOTOPIC(nick, channel)					":IRC 331 " + nick + " " + channel + " :No topic is set\n"
#define RPL_TOPIC(nick, channel, topic) 			":IRC 332 "  + nick + " " + channel + " :" + topic + "\n"
#define RPL_NAMREPLY(nick, channel, users)			":IRC 353 " + nick + " = " + channel + " :" + users + "\n"
#define RPL_ENDOFNAMES(nick, channel)				":IRC 366 " + nick + " " + channel + " :End of /NAMES list\n"
# define RPL_MOTD(nick, comment) 					":IRC 372 " + nick + " :- " + comment + "\n"
# define RPL_MOTDSTART(nick) 						":IRC 375 " + nick + " :- IRC Message of the day - \n"
# define RPL_ENDOFMOTD(nick) 						":IRC 376 " + nick + " :End of MOTD command\n"
# define RPL_YOUREOPER(nick) 						":IRC 381 " + nick + " :You are now an IRC operator\n"

#endif //FT_IRC_ERROR_REPLY_HPP
