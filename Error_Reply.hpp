#ifndef FT_IRC_ERROR_REPLY_HPP
#define FT_IRC_ERROR_REPLY_HPP

/*
 *  Error Replies
 */

#define SERVNAME	"IRKA"

#define ERR_NOSUCHNICK(nick)						":IRC 401 " + nick + " :No such nick/channel\n"
#define ERR_NOSUCHSERVER(servername)				":IRC 402 * " + servername + " :No such server\n"
#define ERR_NOSUCHCHANNEL(nick, channel)			":IRC 403 " + nick + " " + channel + " :No such channel\n"
#define ERR_CANNOTSENDTOCHAN(nick, channel)			":IRC 404 " + nick + " " + channel + " :Cannot send to channel\n"
#define ERR_TOOMANYCHANNELS(nick, channel)			":IRC 405 " + nick + " " + channel + " :You have joined too many channels\n"
#define ERR_WASNOSUCHNICK(nickname)					":IRC 406 * " + nick + " :There was no such nickname\n"
#define ERR_TOOMANYTARGETS(target)					":IRC 407 * " + target + " :Duplicate recipients. No message delivered\n"
#define ERR_NOORIGIN								":IRC 409 * :No origin specified\n"

#define ERR_NORECIPIENT(nick, command)				":IRC 411 " + nick + " :No recipient given " + command + "\n"
#define ERR_NOTEXTTOSEND(nick)						":IRC 412 " + nick + " :No text to send\n"
#define ERR_NOTOPLEVEL(mask)						":IRC 413 * " + mask + " :No toplevel domain specified\n"
#define ERR_WILDTOPLEVEL(mask)						":IRC 414 * " + mask + " :Wildcard in toplevel domain\n"
#define ERR_UNKNOWNCOMMAND(command)					":IRC 421 * " + command + " :Unknown command\n"
#define ERR_NOMOTD									":IRC 422 * :MOTD File is missing\n"
#define ERR_NOADMININFO(server)						":IRC 423 * " + server + " :No administrative info available\n"
#define ERR_FILEERROR(fileop, file)					":IRC 424 * :File errorMain doing " + fileop + " on " + file + "\n"
#define ERR_NONICKNAMEGIVEN							":IRC 431 :No nickname given\n"
#define ERR_ERRONEUSNICKNAME(nick)					":IRC 432 " + nick + " :Erroneus nickname\n"
#define ERR_NICKNAMEINUSE(nick)						":IRC 433 " + nick + " :Nickname is already in use\n"
#define ERR_NICKCOLLISION(nick)						":IRC 436 " + nick + " :Nickname collision KILL\n"
#define ERR_USERNOTINCHANNEL(nick, channel)			":IRC 441 * " + nick + " #" + channel + " :They aren't on that channel\n"
#define ERR_NOTONCHANNEL(nick, channel)				":IRC 442 " + nick + " " + channel + " :You're not on that channel\n"
#define ERR_USERONCHANNEL(nick, channel)			":IRC 443 * " + nick + " #" + channel + " :is already on channel\n"
#define ERR_NOLOGIN(nick)							":IRC 444 * " + nick + " :User not logged in\n"
#define ERR_SUMMONDISABLED							":IRC 445 * :SUMMON has been disabled\n"
#define ERR_USERSDISABLED							":IRC 446 * :USERS has been disabled\n"
#define ERR_NOTREGISTERED							":IRC 451 * :You have not registered\n"
#define ERR_NEEDMOREPARAMS(nick, command)			":IRC 461 " + nick + " " + command + " :Not enough parameters\n"
#define ERR_ALREADYREGISTRED(nick)					":IRC 462 " + nick + " :You may not reregister\n"
#define ERR_NOPERMFORHOST							":IRC 463 * :Your host isn't among the privileged\n"
#define ERR_PASSWDMISMATCH(nick)					":IRC 464 " + nick + " :Password incorrect\n"
#define ERR_YOUREBANNEDCREEP						":IRC 465 * :You are banned from this server\n"
#define ERR_KEYSET(channel)							":IRC 467 * #" + channel + " :Channel key already set\n"
#define ERR_CHANNELISFULL(nick, channel)			":IRC 471 " + nick + " " + channel + " :Cannot join channel (+l)\n"
#define ERR_UNKNOWNMODE(nick, flags)				":IRC 472 " + nick + " " +  flags + " :is unknown mode char to me\n"
#define ERR_INVITEONLYCHAN(channel)					":IRC 473 * #" + channel + " :Cannot join channel (+i)\n"
#define ERR_BANNEDFROMCHAN(nick, channel)			":IRC 474 " + nick + " " + channel + " :Cannot join channel (+b)\n"
#define ERR_BADCHANNELKEY(nick, channel)			":IRC 475 " + nick + " " + channel + " :Cannot join channel (+k)\n"
#define	ERR_BADCHANMASK(channel)					":IRC 476 * #" + channel + " :Bad Channel Mask\n"
#define ERR_NOPRIVILEGES(nick)						":IRC 481 " + nick + " :Permission Denied- You're not an IRC operator\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel)			":IRC 482 " + nick + " " + channel + " :You're not channel operator\n"
#define ERR_CANTKILLSERVER							":IRC 483 * :You cant kill a server!\n"
#define	ERR_RESTRICTED								":IRC 484 * :Your connection is restricted!\n"
#define ERR_NOOPERHOST(nick)						":IRC 491 " + nick + " :No O-lines for your host\n"
#define ERR_UMODEUNKNOWNFLAG(nick, channel)			":IRC 501 " + nick + " " + channel + " :Unknown MODE flag\n"
#define ERR_USERSDONTMATCH(nick)					":IRC 502 " + nick + " :Cant change mode for other users\n"





/*
 *  unknown comms
 */
# define ERR_INVALIDCAP(command) (command + " :Invalid CAP command\n")
# define ERR_NOTOPIC(channel) (channel + " :No topic is set\n")
# define ERR_ALREADYBAN(channel, user) (channel + " " + user + " b :Channel " + channel + " list already contains " + user + "\n")
# define ERR_SERVERISFULL(serv_name) (serv_name + " Server is full\n");
# define ERR_KILLDENY(mssg) (":" + mssg + "\n")



/*
 *  Command responses
 */

#define RPL_WELCOME(nick) 							":IRC 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\n"
# define RPL_UMODEIS(nick, mode) 					":IRC 221 " + nick + " " + mode + "\n"
#define	RPL_AWAY(nick, message)						":IRC 301 * " + nick + " :" + message + "\n"
#define RPL_UNAWAY                              	":IRC 305 * :You are no longer marked as being away\n"
#define RPL_NOWAWAY									":IRC 306 * :You have been marked as being away\n"
#define	RPL_ENDOFWHO								":IRC 315 * :End of WHO list\n"
#define RPL_LISTSTART(nick)							":IRC 321 " + nick + " Channel :Users Name\n"
#define	RPL_LIST(nick, channel, numusers, topic)	":IRC 322 " + nick + " " + channel + " " + numusers + " : " + topic + "\n"
#define RPL_LISTEND(nick)							":IRC 323 " + nick + " :End of /LIST\n"
#define RPL_CHANNELMODEIS(nick, channel, mode)		":IRC 324 " + nick + " " + channel + " " + mode + "\n"
#define	RPL_NOTOPIC(nick, channel)					":IRC 331 " + nick + " " + channel + " :No topic is set\n"
#define RPL_TOPIC(nick, channel, topic) 			":IRC 332 "  + nick + " " + channel + " :" + topic + "\n"
#define	RPL_WHOREPLY(channel, usercount, operator)	":IRC 352 * " + channel + " has " + usercount + " users. Operator: " + operator + "\n"
#define RPL_NAMREPLY(nick, channel, users)			":IRC 353 " + nick + " = " + channel + " :" + users + "\n"
#define RPL_ENDOFNAMES(nick, channel)				":IRC 366 " + nick + " " + channel + " :End of /NAMES list\n"
# define RPL_MOTD(nick, comment) 					":IRC 372 " + nick + " :- " + comment + "\n"
# define RPL_MOTDSTART(nick) 						":IRC 375 " + nick + " :- IRC Message of the day - \n"
# define RPL_ENDOFMOTD(nick) 						":IRC 376 " + nick + " :End of MOTD command\n"
# define RPL_YOUREOPER(nick) 						":IRC 381 " + nick + " :You are now an IRC operator\n"

#define	RPL_TIME(timestr)							":IRC 391 * " + timestr + "\n"

# define RPL_YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version + "\n")
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes + "\n")
# define RPL_CREATED(date) (":This server was created " + date + "\n");
# define RPL_BOUNCE(server_name, port) ("Try server " + server_name + ", port " + port + "\n")
# define RPL_NONE() ("\n") //300
# define RPL_USERHOST(userhost_list) (userhost_list + "\n") //<réponse> ::= <nickname>['*'] '=' <'+'|'-'><hôte>
# define RPL_ISON(user_list) (user_list + "\n")
# define RPL_WHOISUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\n")
//# define RPL_WHOISSERVER(nickname, server) (nickname + " " + server + " :<info serveur>\n")
# define RPL_WHOISOPERATOR(nickname) (nickname + " :is an IRC operator\n")
# define RPL_WHOISIDLE(nickname, idle, signon) (nickname + " " + idle + " " + signon + " :seconds idle, signon time\n")
# define RPL_ENDOFWHOIS(nickname) (nickname + " :End of /WHOIS list\n")
# define RPL_WHOISCHANNELS(nick, chann_info) (nick + " :" + chann_info + "\n")
# define RPL_WHOWASUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\n")
# define RPL_ENDOFWHOWAS(nick) (nick + " :End of WHOWAS\n")


# define RPL_INVITING(channel, nickname) (channel + " " + nickname + "\n")
# define RPL_SUMMONING(user) (user + " :Summoning user to IRC\n")
# define RPL_VERSION(version, debuglevel, server, comment) (version + "." + debuglevel + " " + server + " :" + comment + "\n")
//# define RPL_WHOREPLY(channel, user, host, server, nickname) (channel + " " + user + " " + host + " " + server + " " + nickname + " <H|G>[*][@|+] :<compteur de distance> <vrai nom>\n")
# define RPL_LINKS(mask, server_name, hopcount, info) (mask + " " + server_name + " " + hopcount + " " + info + "\n")
# define RPL_ENDOFLINKS(mask) (mask + " :End of LINKS list\n")
# define RPL_BANLIST(mssg) (mssg + "\n")
# define RPL_ENDOFBANLIST(channel) (channel + " :End of list\n")
# define RPL_INFO(info) (":" + info + "\n")
# define RPL_ENDOFINFO() (":End of INFO list\n")

# define RPL_REHASHING(config_file) (config_file + " :Rehashing\n")
# define RPL_USERSSTART() (":UserID Terminal Hôte\n")
//# define RPL_USERS() (":%-8s %-9s %-8s\n")
# define RPL_ENDOFUSERS() (":End of users\n")
# define RPL_NOUSERS() (":Nobody logged in\n")
# define RPL_TRACELINK(dest, next_serv) ("Link " + std::string(VERSION) + " " + dest + " " + next_serv + "\n")
# define RPL_TRACECONNECTING(classe, server) ("Try. " + classe + " " + server + "\n")
# define RPL_TRACEHANDSHAKE(classe, server) ("H.S. " + classe + " " + serveur + "\n")
# define RPL_TRACEUNKNOWN(nickname) ("???? 1 " + nickname + "\n")
# define RPL_TRACEOPERATOR(nickname) ("Oper 1 " + nickname + "\n")
# define RPL_TRACEUSER(nickname) ("User 1 " + nickname + "\n")
# define RPL_TRACESERVER(server_name) ("Serv 1 0S 0C " + server_name + " *!*@" + server_name + " V" + PROTOCOL_VERSION + "\n")
# define RPL_TRACENEWTYPE(new_type, name) (new_type + " 0 " + name + "\n")
# define RPL_TRACELOG(file_log, debug_level) ("File " + file_log + " " + debug_level + "\n")
# define RPL_TRACEEND(server_name, version_debug) (server_name + " " + version_debug + " :End of TRACE\n")
# define RPL_STATSLINKINFO(arg) (arg + "\n")
# define RPL_STATSCOMMANDS(arg) (arg + "\n")
# define RPL_STATSCLINE(host, name, port, classe) ("C " + host + " * " + name + " " + port + " " + classe + "\n")
# define RPL_STATSNLINE(host, name, port, classe) ("N " + host + " * " + name + " " + port + " " + classe + "\n")
# define RPL_STATSILINE(host, port, classe) ("I " + host + " * " + host + " " + port + " " + classe + "\n")
# define RPL_STATSKLINE(host, user_name, port, classe) ("K " + host + " * " + user_name + " " + port + " " + classe + "\n")
# define RPL_STATSYLINE(classe, frq_ping, frq_connection, sendq_max) ("Y " + classe + " " + frq_ping + " " + frq_connection + " " + sendq_max + "\n")
# define RPL_ENDOFSTATS(stat_letter) (stat_letter + " :End of STATS report\n")
# define RPL_STATSLLINE(host_mask, server_name, max_depth) ("L " + host_mask + " * " + server_name + max_depth "\n")
# define RPL_STATSUPTIME(arg) (":Server Up " + arg + "\n")
# define RPL_STATSOLINE(host_mask, name) ("O " + host_mask + " * " + name + "\n")
# define RPL_STATSHLINE(host_mask, server_name) ("H " + host_mask + " * " + server_name + "\n")

# define RPL_LUSERCLIENT(int1, int2, int3) (":There are " + int1 + " users and " + int2 + " invisible on " + int3 + " servers\n")
# define RPL_LUSEROP(int1) (int1 + " :operator(s) online\n")
# define RPL_LUSERUNKNOWN(int1) (int1 + " :unknown connection(s)\n")
# define RPL_LUSERCHANNELS(int1) (int1 + " :channels formed\n")
# define RPL_LUSERME(int1, int2) (":I have " + int1 + " clients and " + int2 + " servers\n")
# define RPL_ADMINME(server) (server + " :Administrative info\n")
# define RPL_ADMINLOC1(info) (":" + info + "\n")
# define RPL_ADMINLOC2(info) (":" + info + "\n")
# define RPL_ADMINEMAIL(info) (":" + info + "\n")
# define RPL_CREATIONTIME(channel, c_time) (channel + " " + c_time + "\n")
# define RPL_YOURESERVICE(servicename) (":You are service " + servicename + "\n")


#endif //FT_IRC_ERROR_REPLY_HPP
