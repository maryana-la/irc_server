#ifndef FT_IRC_ERROR_REPLY_HPP
#define FT_IRC_ERROR_REPLY_HPP

/*
 *  Error Replies
 */

#define SERVNAME	"IRKA"

# define ERR_NOSUCHNICK(nickname) ((nickname) + " :No such nick/channel\r\n")  //401
# define ERR_NOSUCHSERVER(server_name) ((server_name) + " :No such server\r\n")  //402
# define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel\r\n")  //403
# define ERR_CANNOTSENDTOCHAN(channel) (channel + " :Cannot send to channel\r\n")  //404
# define ERR_TOOMANYCHANNELS(channel) (channel + " :You have joined too many channels\r\n")  //405
# define ERR_WASNOSUCHNICK(nickname) ((nickname) + " :There was no such nickname\r\n")  //406
# define ERR_TOOMANYTARGETS(target) ((target) + ":Duplicate recipients. No message delivered\r\n")  //407
# define ERR_NOORIGIN() (":No origin specified\r\n") //409
# define ERR_NORECIPIENT(command) (":No recipient given (" + command + ")\r\n")  //411
# define ERR_NOTEXTTOSEND() (":No text to send\r\n")  //412
# define ERR_NOTOPLEVEL(mask) (mask + " :No toplevel domain specified\r\n")  //413
# define ERR_WILDTOPLEVEL(mask) (mask + " :Wildcard in toplevel domain\r\n")  //414
# define ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command\r\n")  //421
# define ERR_NOMOTD() (":MOTD File is missing\r\n")  //422
# define ERR_NOADMININFO(server_name) (server_name + " :No administrative info available\r\n")  //423
# define ERR_FILEERROR(operation, file) (":File error doing " + operation + " on " + file + "\r\n")  //424
# define ERR_NONICKNAMEGIVEN() (":No nickname given\r\n")  //431
# define ERR_ERRONEUSNICKNAME(nick) ((nick) + " :Erroneus nickname\r\n")  //432
# define ERR_NICKNAMEINUSE(nick) ((nick) + " :Nickname is already in use\r\n")  //433
# define ERR_NICKCOLLISION(nick) ((nick) + " :Nickname collision KILL\r\n")  //436


# define ERR_USERNOTINCHANNEL(nickname, channel) ((nick) + " " + (channel) + " :They aren't on that channel\r\n")  //441
# define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel\r\n")  //442
# define ERR_USERONCHANNEL(user, channel) ((user) + " " + (channel) + " :is already on channel")  //443
# define ERR_NOLOGIN(user) ((user) + " :User not logged in\r\n")  //444
# define ERR_SUMMONDISABLED() (":SUMMON has been disabled\r\n")  //445
# define ERR_USERSDISABLED() (":USERS has been disabled\r\n")  //446
# define ERR_NOTREGISTERED() (":You have not registered\r\n")
# define ERR_NEEDMOREPARAMS(command)		((command) + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTRED()				(":Unauthorized command (already registered)\r\n")
# define ERR_NOPERMFORHOST() (":Your host isn't among the privileged\r\n")
# define ERR_PASSWDMISMATCH() (":Password incorrect\r\n")
# define ERR_YOUREBANNEDCREEP() (":You are banned from this server\r\n")
# define ERR_KEYSET(channel) ((channel) + " :Channel key already set\r\n")
# define ERR_CHANNELISFULL(channel) (channel + " :Cannot join channel (+l)\r\n")
# define ERR_UNKNOWNMODE(character) (character + " :is unknown mode char to me\r\n")
# define ERR_INVITEONLYCHAN(channel) (channel + " :Cannot join channel (+i)\r\n")
# define ERR_BANNEDFROMCHAN(channel) (channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(channel) (channel + " :Cannot join channel (+k)\r\n")
# define ERR_NOPRIVILEGES() (":Permission Denied- You're not an IRC operator\r\n")
# define ERR_CHANOPRIVSNEEDED(channel) (channel + " :You're not channel operator\r\n")
# define ERR_CANTKILLSERVER() (":You cant kill a server!\r\n")
# define ERR_NOOPERHOST() (":No O-lines for your host\r\n")
# define ERR_UMODEUNKNOWNFLAG() (":Unknown MODE flag\r\n")
# define ERR_USERSDONTMATCH() (":Cannot change mode for other users\r\n")

/*
 * unknown comms
 */
# define ERR_INVALIDCAP(command) (command + " :Invalid CAP command\r\n")
# define ERR_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define ERR_BADCHANMASK(channel) (channel + " :Bad Channel Mask\r\n")
# define ERR_ALREADYBAN(channel, user) (channel + " " + user + " b :Channel " + channel + " list already contains " + user + "\r\n")
# define ERR_SERVERISFULL(serv_name) (serv_name + " Server is full\r\n");
# define ERR_KILLDENY(mssg) (":" + mssg + "\r\n")



/*
**  Command responses
*/
# define RPL_WELCOME(nick) (":Welcome to the Internet Relay Network " + nick + "\r\n")
# define RPL_YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version + "\r\n")
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes + "\r\n")
# define RPL_CREATED(date) (":This server was created " + date + "\r\n");
# define RPL_BOUNCE(server_name, port) ("Try server " + server_name + ", port " + port + "\r\n")
# define RPL_NONE() ("\r\n") //300
# define RPL_USERHOST(userhost_list) (userhost_list + "\r\n") //<réponse> ::= <nickname>['*'] '=' <'+'|'-'><hôte>
# define RPL_ISON(user_list) (user_list + "\r\n")
# define RPL_AWAY(nickname, str) (nickname + " " + str + "\r\n")
# define RPL_UNAWAY() (":You are no longer marked as being away\r\n")
# define RPL_NOWAWAY() (":You have been marked as being away\r\n")
# define RPL_WHOISUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\r\n")
//# define RPL_WHOISSERVER(nickname, server) (nickname + " " + server + " :<info serveur>\r\n")
# define RPL_WHOISOPERATOR(nickname) (nickname + " :is an IRC operator\r\n")
# define RPL_WHOISIDLE(nickname, idle, signon) (nickname + " " + idle + " " + signon + " :seconds idle, signon time\r\n")
# define RPL_WHOREPLY(arg) (arg + "\r\n")
# define RPL_ENDOFWHOIS(nickname) (nickname + " :End of /WHOIS list\r\n")
# define RPL_WHOISCHANNELS(nick, chann_info) (nick + " :" + chann_info + "\r\n")
# define RPL_WHOWASUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\r\n")
# define RPL_ENDOFWHOWAS(nick) (nick + " :End of WHOWAS\r\n")
# define RPL_LISTSTART() ("Channel :Users Name\r\n")
# define RPL_LIST(channel, topic) (channel + " :" + topic + "\r\n")
# define RPL_LISTEND() (":End of LIST\r\n")
# define RPL_CHANNELMODEIS(channel, mode, params) (channel + " " + mode + " " + params + "\r\n")
# define RPL_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define RPL_TOPIC(channel, subject) (channel + " :" + subject + "\r\n")
# define RPL_INVITING(channel, nickname) (channel + " " + nickname + "\r\n")
# define RPL_SUMMONING(user) (user + " :Summoning user to IRC\r\n")
# define RPL_VERSION(version, debuglevel, server, comment) (version + "." + debuglevel + " " + server + " :" + comment + "\r\n")
//# define RPL_WHOREPLY(channel, user, host, server, nickname) (channel + " " + user + " " + host + " " + server + " " + nickname + " <H|G>[*][@|+] :<compteur de distance> <vrai nom>\r\n")
# define RPL_ENDOFWHO(name) (name + " :End of WHO list\r\n")
# define RPL_NAMREPLY(channel, nick_list) (channel + " :" + nick_list + "\r\n") //modif_ici
# define RPL_ENDOFNAMES(channel) (channel + " :End of NAMES list\r\n")
# define RPL_LINKS(mask, server_name, hopcount, info) (mask + " " + server_name + " " + hopcount + " " + info + "\r\n")
# define RPL_ENDOFLINKS(mask) (mask + " :End of LINKS list\r\n")
# define RPL_BANLIST(mssg) (mssg + "\r\n")
# define RPL_ENDOFBANLIST(channel) (channel + " :End of list\r\n")
# define RPL_INFO(info) (":" + info + "\r\n")
# define RPL_ENDOFINFO() (":End of INFO list\r\n")
# define RPL_MOTDSTART(nick) (":" + "SERVNAME" + " 375 " + (nick) + " :- " + "SERVNAME" + " Message of the day - \r\n")
# define RPL_MOTD(nick, comment) (":" + "SERVNAME" + " 372 " + (nick) + " :- " + comment + "\r\n")
# define RPL_ENDOFMOTD(nick) (":" + "SERVNAME" + " 375 " + nick + ":End of MOTD command\r\n")
# define RPL_YOUREOPER() (":You are now an IRC operator\r\n")
# define RPL_REHASHING(config_file) (config_file + " :Rehashing\r\n")
# define RPL_TIME(server, time_serveur) (server + " :" + time_serveur + "\r\n")
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
