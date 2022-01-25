#include "Server.hpp"

void Server::listExec(Client &client, std::vector<std::string> &args) {
	sendMessage(RPL_LISTSTART(client.getNick()), client.getSockFd());
	/* if list without arguments */
	if (args.size() == 1) {
		std::vector<Channel*>::iterator it = _channels.begin();
		std::vector<Channel*>::iterator ite = _channels.end();
		for(; it !=ite; it++)
			sendMessage(RPL_LIST(client.getNick(),(*it)->getChannelName(),  intToString((*it)->getNumUsers()),(*it)->getTopic()), client.getSockFd());
	}
		/* if channels specified */
	else if (args.size() == 2) {
		std::vector<std::string> channs = split(args[1], ",");
		std::vector<std::string>::iterator itCh = channs.begin();
		std::vector<std::string>::iterator iteCh = channs.end();
		for (; itCh != iteCh; itCh++) {
			if (Channel* tmp = findChannel(*itCh))
				sendMessage(RPL_LIST(client.getNick(),tmp->getChannelName(), intToString((tmp)->getNumUsers()), tmp->getTopic()), client.getSockFd());
		}
	}
	sendMessage(RPL_LISTEND(client.getNick()), client.getSockFd());
}

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
			}
		}
	}
}

void Server::pingExec(Client &client, std::vector<std::string> &args){
	sendMessage(":IRC PONG " + args[1] + "\n", client.getSockFd() );
}

void Server::pongExec(Client &client, std::vector<std::string> &args){
	sendMessage(":IRC PING " + args[1] + "\n", client.getSockFd() );
}
