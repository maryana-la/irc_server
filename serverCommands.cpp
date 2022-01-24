#include "Server.hpp"

void Server::namesExec(Client &client, std::vector<std::string> &args) {
	/* if list without arguments */
	if (args.size() == 1) {
		std::vector<Channel*>::iterator it = _channels.begin();
		std::vector<Channel*>::iterator ite = _channels.end();
		/* send RPL_NAMREPLY for each channel */
		for(; it !=ite; it++)
			sendUsers(client, *(*it));

		/* send clients that are not in any channel */
		std::vector<Client*>::iterator itCli = _users.begin();
		std::string noChannelUsers;
		for (;itCli != _users.end(); itCli++) {  //iterate by users
			it = _channels.begin();
			for (; it != ite; it++) {  //iterate by channels
				if ((*it)->isChannelUser(*itCli))  //if user is a member of some channel
					break;
			}
			/* make string with clients' nick */
			if (noChannelUsers.empty() && it == ite && !(*itCli)->getInvisibleStatus())
				noChannelUsers += (*itCli)->getNick();
			else if (!noChannelUsers.empty() && it == ite && !(*itCli)->getInvisibleStatus())
				noChannelUsers += " " + (*itCli)->getNick();
		}
		sendMessage(RPL_NAMREPLY(client.getNick(), "*", noChannelUsers), client.getSockFd());
		sendMessage(RPL_ENDOFNAMES(client.getNick(), "*"), client.getSockFd());
	}
	/* if channels specified */
	else if (args.size() == 2) {
		std::vector<std::string> channs = split(args[1], ",");
		std::vector<std::string>::iterator itCh = channs.begin();
		std::vector<std::string>::iterator iteCh = channs.end();
		for (; itCh != iteCh; itCh++) {
			if (Channel* tmp = findChannel(*itCh)) {
				sendUsers(client, *tmp);
//				sendMessage(RPL_ENDOFNAMES(client.getNick(), "*"), client.getSockFd());
			}
		}
	}
//	sendMessage(RPL_ENDOFNAMES(client.getNick(), "*"), client.getSockFd());
}

void Server::operExec(Client &client, std::vector<std::string> &args) {
	/* check number of args */
	if (args.size() != 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
	/* if no operators allowed on server */
	if (_operator_login.empty())
		throw static_cast<std::string>(ERR_NOOPERHOST(client.getNick()));
	/* compare received data with valid logins */
	std::map<std::string, std::string>::iterator it = _operator_login.begin();
	std::map<std::string, std::string>::iterator ite = _operator_login.end();
	for (; it != ite; it++) {
		if ((*it).first == args[1] && (*it).second == args[2]) {
			_operators.insert(&client);  //add new operator
			sendMessage(RPL_YOUREOPER(client.getNick()), client.getSockFd());
			return;
		}
	}
	throw  static_cast<std::string>(ERR_PASSWDMISMATCH(client.getNick()));
}
//
//void Server::informOfNewOperator(Client &client) {
//	std::vector<Client*>::iterator it = _users.begin();
//	std::vector<Client*>::iterator ite = _users.end();
//	for(; it !=ite; it++)
//		sendMessage(RPL_UMODEIS(client.getNick(), "[+o]"), (*it)->getSockFd());
//}

void Server::quitExec(Client &client, std::vector<std::string> &args) {
	/* make a leave message */
//	Client *tmp = client;
	std::string leaveMessage;
	if (args.size() > 1)
		leaveMessage = " :" + args[1];
	else
		leaveMessage = ":Client exited";
	std::vector<Channel*>::iterator itCh = _channels.begin();
	std::vector<Channel*>::iterator iteCh = _channels.end();
	/* leave from every channel */
	for (; itCh != iteCh; itCh++) {
		if ((*itCh)->isChannelUser(&client)) { // if client is a channel user, quit channel
			standartReply(client, *itCh, "QUIT", leaveMessage);
			leaveChannel(client, *itCh);
		}
	}

	/* remove user and close fd */
	removeClient(&client);
	removeOperator(&client);
	close(client.getSockFd());
	std::vector<pollfd>::iterator it = _fds.begin();
	std::vector<pollfd>::iterator ite = _fds.end();
	for ( ; it != ite; it++) {
		if (client.getSockFd() == (*it).fd) {
			_fds.erase(it);
			break ;
		}
	}
	std::cout << client.getNick() << " has quited\n";
}

void Server::forceQuit(Client &client, std::string args) {
	std::string msg = prefixCompose(client) + "QUIT ::Connection lost\n";
	std::vector<Channel*>::iterator itCh = _channels.begin();
	std::vector<Channel*>::iterator iteCh = _channels.end();
	/* leave from every channel */
	for (; itCh != iteCh; itCh++) {
		if ((*itCh)->isChannelUser(&client)) { // if client is a channel user, quit channel
			(*itCh)->sendMsgToChan(msg, &client, false);
			leaveChannel(client, *itCh);
		}
	}
	/* remove user and close fd */
	removeClient(&client);
	removeOperator(&client);
	close(client.getSockFd());
	std::vector<pollfd>::iterator it = _fds.begin();
	std::vector<pollfd>::iterator ite = _fds.end();
	for ( ; it != ite; it++) {
		if (client.getSockFd() == (*it).fd) {
			_fds.erase(it);
			break ;
		}
	}
	std::cout << client.getNick() << " has lost connection\n";
}

void Server::killExec(Client &client, std::vector<std::string> &args) {
	/* check number of args */
	if (args.size() < 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
	/* check if client is IRC-operator */
	if (!isServerOperator(&client))
		throw static_cast<std::string>(ERR_NOPRIVILEGES(client.getNick()));
	/* find client to kill */
	Client *toKill = findClient(args[1]);
	if (!toKill)
		throw static_cast<std::string>(ERR_NOSUCHNICK(args[1]));
	args.erase(args.begin());
	quitExec(*toKill, args);
	std::cout << toKill->getNick() << " killed due to " << args[2] << std::endl;
}

void Server::exitExec(Client &client) {
	if (isServerOperator(&client)) {
		std::string msg = prefixCompose(client) + "EXIT :server stopped\n";
		std::vector<Client *>::iterator it = _users.begin();
		for (;it != _users.end(); it++) {
			sendMessage(msg, (*it)->getSockFd());
		}
		sleep(5);
		exit(1);
	}
}