#ifndef FT_IRC_ERROR_REPLY_HPP
#define FT_IRC_ERROR_REPLY_HPP

/*
 *  Error Replies
 */

#define SERVNAME	"IRKA"

#define ERR_NOSUCHNICK(nick)						":IRC 401 " + nick + " :No such nick/channel\r\n"
#define ERR_NOSUCHSERVER(servername)				":IRC 402 * " + servername + " :No such server\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel)			":IRC 403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(nick, channel)			":IRC 404 " + nick + " " + channel + " :Cannot send to channel\r\n"
#define ERR_TOOMANYCHANNELS(nick, channel)			":IRC 405 " + nick + " " + channel + " :You have joined too many channels\r\n"
#define ERR_WASNOSUCHNICK(nickname)					":IRC 406 * " + nick + " :There was no such nickname\r\n"
#define ERR_TOOMANYTARGETS(target)					":IRC 407 * " + target + " :Duplicate recipients. No message delivered\r\n"
#define ERR_NOORIGIN								":IRC 409 * :No origin specified\r\n"

#define ERR_NORECIPIENT(nick, command)				":IRC 411 " + nick + " :No recipient given " + command + "\r\n"
#define ERR_NOTEXTTOSEND(nick)						":IRC 412 " + nick + " :No text to send\r\n"
#define ERR_NOTOPLEVEL(mask)						":IRC 413 * " + mask + " :No toplevel domain specified\r\n"
#define ERR_WILDTOPLEVEL(mask)						":IRC 414 * " + mask + " :Wildcard in toplevel domain\r\n"
#define ERR_UNKNOWNCOMMAND(command)					":IRC 421 * " + command + " :Unknown command\r\n"
#define ERR_NOMOTD									":IRC 422 * :MOTD File is missing\r\n"
#define ERR_NOADMININFO(server)						":IRC 423 * " + server + " :No administrative info available\r\n"
#define ERR_FILEERROR(fileop, file)					":IRC 424 * :File error doing " + fileop + " on " + file + "\r\n"
#define ERR_NONICKNAMEGIVEN							":IRC 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick)					":IRC 432 " + nick + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick)						":IRC 433 " + nick + " :Nickname is already in use\r\n"
#define ERR_NICKCOLLISION(nick)						":IRC 436 " + nick + " :Nickname collision KILL\r\n"
#define ERR_USERNOTINCHANNEL(nick, channel)			":IRC 441 * " + nick + " #" + channel + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(nick, channel)				":IRC 442 " + nick + " " + channel + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(nick, channel)			":IRC 443 * " + nick + " #" + channel + " :is already on channel\r\n"
#define ERR_NOLOGIN(nick)							":IRC 444 * " + nick + " :User not logged in\r\n"
#define ERR_SUMMONDISABLED							":IRC 445 * :SUMMON has been disabled\r\n"
#define ERR_USERSDISABLED							":IRC 446 * :USERS has been disabled\r\n"
#define ERR_NOTREGISTERED							":IRC 451 * :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(nick, command)			":IRC 461 " + nick + " " + command + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED(nick)					":IRC 462 " + nick + " :You may not reregister\r\n"
#define ERR_NOPERMFORHOST							":IRC 463 * :Your host isn't among the privileged\r\n"
#define ERR_PASSWDMISMATCH							":IRC 464 * :Password incorrect\r\n"
#define ERR_YOUREBANNEDCREEP						":IRC 465 * :You are banned from this server\r\n"
#define ERR_KEYSET(channel)							":IRC 467 * #" + channel + " :Channel key already set\r\n"
#define ERR_CHANNELISFULL(nick, channel)			":IRC 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE(nick, char)					":IRC 472 " + nick + " " +  char + " :is unknown mode char to me\r\n"
#define ERR_INVITEONLYCHAN(channel)					":IRC 473 * #" + channel + " :Cannot join channel (+i)\r\n"
#define ERR_BANNEDFROMCHAN(nick, channel)			":IRC 474 " + nick + " " + channel + " :Cannot join channel (+b)\r\n"
#define ERR_BADCHANNELKEY(nick, channel)			":IRC 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n"
#define	ERR_BADCHANMASK(channel)					":IRC 476 * #" + channel + " :Bad Channel Mask\r\n"
#define ERR_NOPRIVILEGES							":IRC 481 * :Permission Denied- You're not an IRC operator\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel)			":IRC 482 " + nick + " " + channel + " :You're not channel operator\r\n"
#define ERR_CANTKILLSERVER							":IRC 483 * :You cant kill a server!\r\n"
#define	ERR_RESTRICTED								":IRC 484 * :Your connection is restricted!\r\n"
#define ERR_NOOPERHOST								":IRC 491 * :No O-lines for your host\r\n"
#define ERR_UMODEUNKNOWNFLAG(nick, channel)			":IRC 501 " + nick + " " + channel + " :Unknown MODE flag\r\n"
#define ERR_USERSDONTMATCH(nick, channel)			":IRC 502 " + nick + " " + channel + " :Cant change mode for other users\r\n"





/*
 * unknown comms
 */
# define ERR_INVALIDCAP(command) (command + " :Invalid CAP command\r\n")
# define ERR_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define ERR_ALREADYBAN(channel, user) (channel + " " + user + " b :Channel " + channel + " list already contains " + user + "\r\n")
# define ERR_SERVERISFULL(serv_name) (serv_name + " Server is full\r\n");
# define ERR_KILLDENY(mssg) (":" + mssg + "\r\n")



/*
**  Command responses
*/

#define RPL_WELCOME(nick) 							":IRC 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\r\n"
#define	RPL_AWAY(nick, message)						":IRC 301 * " + nick + " :" + message + "\r\n"
#define RPL_UNAWAY                              	":IRC 305 * :You are no longer marked as being away\r\n"
#define RPL_NOWAWAY									":IRC 306 * :You have been marked as being away\r\n"
#define	RPL_ENDOFWHO								":IRC 315 * :End of WHO list\r\n"
#define RPL_LISTSTART(nick)							":IRC 321 " + nick + " Channel :Users Name\r\n"
#define	RPL_LIST(nick, channel, numusers, topic)	":IRC 322 " + nick + " " + channel + " " + numusers + " : " + topic + "\r\n"
#define RPL_LISTEND(nick)							":IRC 323 " + nick + " :End of /LIST\r\n"
#define RPL_CHANNELMODEIS(nick, channel, mode)		":IRC 324 " + nick + " " + channel + " " + mode + "\r\n"
#define	RPL_NOTOPIC(nick, channel)					":IRC 331 " + nick + " " + channel + " :No topic is set\r\n"
#define RPL_TOPIC(nick, channel, topic) 			":IRC 332 "  + nick + " " + channel + " :" + topic + "\r\n"
#define	RPL_WHOREPLY(channel, usercount, operator)	":IRC 352 * " + channel + " has " + usercount + " users. Operator: " + operator + "\r\n"
#define RPL_NAMREPLY(nick, channel, users)			":IRC 353 " + nick + " " + channel + " = :" + users + "\r\n"
#define RPL_ENDOFNAMES(nick, channel)				":IRC 366 " + nick + " " + channel + " :End of /NAMES list\r\n"

# define RPL_MOTD(nick, comment) 					":IRC 372 " + (nick) + " :- " + comment + "\r\n"
# define RPL_MOTDSTART(nick) 						":IRC 375 " + (nick) + " :- IRC Message of the day - \r\n"
# define RPL_ENDOFMOTD(nick) 						":IRC 376 " + (nick) + " :End of MOTD command\r\n"



#define	RPL_TIME(timestr)							":IRC 391 * " + timestr + "\r\n"

# define RPL_YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version + "\r\n")
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes + "\r\n")
# define RPL_CREATED(date) (":This server was created " + date + "\r\n");
# define RPL_BOUNCE(server_name, port) ("Try server " + server_name + ", port " + port + "\r\n")
# define RPL_NONE() ("\r\n") //300
# define RPL_USERHOST(userhost_list) (userhost_list + "\r\n") //<réponse> ::= <nickname>['*'] '=' <'+'|'-'><hôte>
# define RPL_ISON(user_list) (user_list + "\r\n")
# define RPL_WHOISUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\r\n")
//# define RPL_WHOISSERVER(nickname, server) (nickname + " " + server + " :<info serveur>\r\n")
# define RPL_WHOISOPERATOR(nickname) (nickname + " :is an IRC operator\r\n")
# define RPL_WHOISIDLE(nickname, idle, signon) (nickname + " " + idle + " " + signon + " :seconds idle, signon time\r\n")
# define RPL_ENDOFWHOIS(nickname) (nickname + " :End of /WHOIS list\r\n")
# define RPL_WHOISCHANNELS(nick, chann_info) (nick + " :" + chann_info + "\r\n")
# define RPL_WHOWASUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\r\n")
# define RPL_ENDOFWHOWAS(nick) (nick + " :End of WHOWAS\r\n")


# define RPL_INVITING(channel, nickname) (channel + " " + nickname + "\r\n")
# define RPL_SUMMONING(user) (user + " :Summoning user to IRC\r\n")
# define RPL_VERSION(version, debuglevel, server, comment) (version + "." + debuglevel + " " + server + " :" + comment + "\r\n")
//# define RPL_WHOREPLY(channel, user, host, server, nickname) (channel + " " + user + " " + host + " " + server + " " + nickname + " <H|G>[*][@|+] :<compteur de distance> <vrai nom>\r\n")
# define RPL_LINKS(mask, server_name, hopcount, info) (mask + " " + server_name + " " + hopcount + " " + info + "\r\n")
# define RPL_ENDOFLINKS(mask) (mask + " :End of LINKS list\r\n")
# define RPL_BANLIST(mssg) (mssg + "\r\n")
# define RPL_ENDOFBANLIST(channel) (channel + " :End of list\r\n")
# define RPL_INFO(info) (":" + info + "\r\n")
# define RPL_ENDOFINFO() (":End of INFO list\r\n")
# define RPL_YOUREOPER() (":You are now an IRC operator\r\n")
# define RPL_REHASHING(config_file) (config_file + " :Rehashing\r\n")
# define RPL_USERSSTART() (":UserID Terminal Hôte\r\n")
//# define RPL_USERS() (":%-8s %-9s %-8s\r\n")
# define RPL_ENDOFUSERS() (":End of users\r\n")
# define RPL_NOUSERS() (":Nobody logged in\r\n")
# define RPL_TRACELINK(dest, next_serv) ("Link " + std::string(VERSION) + " " + dest + " " + next_serv + "\r\n")
# define RPL_TRACECONNECTING(classe, server) ("Try. " + classe + " " + server + "\r\n")
# define RPL_TRACEHANDSHAKE(classe, server) ("H.S. " + classe + " " + serveur + "\r\n")
# define RPL_TRACEUNKNOWN(nickname) ("???? 1 " + nickname + "\r\n")
# define RPL_TRACEOPERATOR(nickname) ("Oper 1 " + nickname + "\r\n")
# define RPL_TRACEUSER(nickname) ("User 1 " + nickname + "\r\n")
# define RPL_TRACESERVER(server_name) ("Serv 1 0S 0C " + server_name + " *!*@" + server_name + " V" + PROTOCOL_VERSION + "\r\n")
# define RPL_TRACENEWTYPE(new_type, name) (new_type + " 0 " + name + "\r\n")
# define RPL_TRACELOG(file_log, debug_level) ("File " + file_log + " " + debug_level + "\r\n")
# define RPL_TRACEEND(server_name, version_debug) (server_name + " " + version_debug + " :End of TRACE\r\n")
# define RPL_STATSLINKINFO(arg) (arg + "\r\n")
# define RPL_STATSCOMMANDS(arg) (arg + "\r\n")
# define RPL_STATSCLINE(host, name, port, classe) ("C " + host + " * " + name + " " + port + " " + classe + "\r\n")
# define RPL_STATSNLINE(host, name, port, classe) ("N " + host + " * " + name + " " + port + " " + classe + "\r\n")
# define RPL_STATSILINE(host, port, classe) ("I " + host + " * " + host + " " + port + " " + classe + "\r\n")
# define RPL_STATSKLINE(host, user_name, port, classe) ("K " + host + " * " + user_name + " " + port + " " + classe + "\r\n")
# define RPL_STATSYLINE(classe, frq_ping, frq_connection, sendq_max) ("Y " + classe + " " + frq_ping + " " + frq_connection + " " + sendq_max + "\r\n")
# define RPL_ENDOFSTATS(stat_letter) (stat_letter + " :End of STATS report\r\n")
# define RPL_STATSLLINE(host_mask, server_name, max_depth) ("L " + host_mask + " * " + server_name + max_depth "\r\n")
# define RPL_STATSUPTIME(arg) (":Server Up " + arg + "\r\n")
# define RPL_STATSOLINE(host_mask, name) ("O " + host_mask + " * " + name + "\r\n")
# define RPL_STATSHLINE(host_mask, server_name) ("H " + host_mask + " * " + server_name + "\r\n")
# define RPL_UMODEIS(user_mode) (user_mode + "\r\n")
# define RPL_LUSERCLIENT(int1, int2, int3) (":There are " + int1 + " users and " + int2 + " invisible on " + int3 + " servers\r\n")
# define RPL_LUSEROP(int1) (int1 + " :operator(s) online\r\n")
# define RPL_LUSERUNKNOWN(int1) (int1 + " :unknown connection(s)\r\n")
# define RPL_LUSERCHANNELS(int1) (int1 + " :channels formed\r\n")
# define RPL_LUSERME(int1, int2) (":I have " + int1 + " clients and " + int2 + " servers\r\n")
# define RPL_ADMINME(server) (server + " :Administrative info\r\n")
# define RPL_ADMINLOC1(info) (":" + info + "\r\n")
# define RPL_ADMINLOC2(info) (":" + info + "\r\n")
# define RPL_ADMINEMAIL(info) (":" + info + "\r\n")
# define RPL_CREATIONTIME(channel, c_time) (channel + " " + c_time + "\r\n")
# define RPL_YOURESERVICE(servicename) (":You are service " + servicename + "\r\n")


#endif //FT_IRC_ERROR_REPLY_HPP
