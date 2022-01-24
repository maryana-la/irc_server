#include "Server.hpp"


void Server::modeExec(Client &client, std::vector<std::string> &args) {
	/* check number of args */
	if (args.size() < 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
	/* check number if +- in flag argument */
	if (args[2][0] != '+' && args[2][0] != '-')
		throw static_cast<std::string>(ERR_UNKNOWNMODE(client.getNick(), args[2]));


	/* if channel */
	if (args[1].find_first_of("&#+!") == 0) {
		if (args[2].find_first_not_of("+-otlksn") != std::string::npos)
			throw static_cast<std::string>(ERR_UNKNOWNMODE(client.getNick(), args[2]));
		setChannelModes(client, args);
	}
	/* if user */
	else {
		if (args[2].find_first_not_of("+-oi") != std::string::npos) /* check if flags are valid */
			throw static_cast<std::string>(ERR_UMODEUNKNOWNFLAG(client.getNick(), args[1]));
		setUserModes(client, args);
	}
}

void Server::setChannelModes(Client &client, std::vector<std::string> &args) {
	/* check if requested channel exists */
	Channel *ChanToUpd = findChannel(args[1]);
	if (!ChanToUpd)
		throw static_cast<std::string>(ERR_NOSUCHCHANNEL(client.getNick(), args[1]));

	/* check if client is operator */
	if (!ChanToUpd->isOperator(&client))
		throw static_cast<std::string>(ERR_CHANOPRIVSNEEDED(client.getNick(), ChanToUpd->getChannelName()));

	bool isPlus = (args[2][0] == '+');
	Client *ClientToUpd;
	std::string::iterator it = args[2].begin() + 1;
	for (; it != args[2].end(); it++) {
		switch (*it) {
			case 'o':
				if (args.size() != 4)
					throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
				ClientToUpd = findClient(args[3]);
				if (!ClientToUpd)  // if requested user exists
					throw static_cast<std::string>(ERR_NOSUCHNICK(args[3]));
				if (!ChanToUpd->isChannelUser(ClientToUpd))  // if requested user a member of channel
					throw static_cast<std::string>(ERR_USERSDONTMATCH(client.getNick()));
				/* add/delete new operator of the channel */
				if (isPlus)
					ChanToUpd->addOperator(*ClientToUpd);
				else
					ChanToUpd->deleteOperator(*ClientToUpd);
				standartReply(client, ChanToUpd, "MODE " + ChanToUpd->getChannelName(), (isPlus ? "+" : "-") + (std::string)"o " + ClientToUpd->getNick());
				break;
			case 't': /* topic can be set only by operator */
				ChanToUpd->setTopicOperOnly(isPlus);
				standartReply(client, ChanToUpd, "MODE " + ChanToUpd->getChannelName(), + (isPlus ? "+" : "-") + (std::string)"t");
				break;
			case 'l': /* set limit of users in th channel */
				if (isPlus) {
					if (args.size() != 4)
						throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
					ChanToUpd->setMaxUsers(strtol(args[3].c_str(), NULL, 10));
					ChanToUpd->setMaxUsersFlag(true);
				}
				else
					ChanToUpd->setMaxUsersFlag(false);
				standartReply(client, ChanToUpd, "MODE " + ChanToUpd->getChannelName(), + (isPlus ? "+" : "-") + (std::string)"l " + intToString(ChanToUpd->getMaxUsers()));
				break;
			case 'k': /* set key to enter channel */
				if (isPlus) {
					if (args.size() != 4)
						throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
					ChanToUpd->setKey(args[3]);
					ChanToUpd->setKeyFlag(isPlus);
				}
				else
					ChanToUpd->setKeyFlag(isPlus);
				standartReply(client, ChanToUpd, "MODE " + ChanToUpd->getChannelName(), + (isPlus ? "+" : "-") + (std::string)"k " + ChanToUpd->getKey());
				break;
		}
	}
	/* make string for RPL */ ///should be by standard, but not working for lime chat
//	std::string modes = "[+n";
//	if (ChanToUpd->getTopicOperatorsOnly())
//		modes += "t";
//	if (ChanToUpd->getKeyStatus())
//		modes += "k";
//	if (ChanToUpd->getMaxUsersFlag())
//		modes += "l(" + intToString(ChanToUpd->getMaxUsers()) + ")";
//	modes += "]";
//	sendMessage(RPL_CHANNELMODEIS(client.getNick(), ChanToUpd->getChannelName(), modes), client.getSockFd());
}

void Server::setUserModes(Client &client, std::vector<std::string> &args) {
	Client *ClientToUpd = findClient(args[1]);
	if (!ClientToUpd)
		throw static_cast<std::string>(ERR_NOSUCHNICK(args[1]));
	std::string::iterator it = args[2].begin() + 1;
	for (; it != args[2].end(); it++) {
		switch (*it) {
			case 'o':
				if (!isServerOperator(&client)) /* check if requester is operator */
					throw static_cast<std::string>(ERR_USERSDONTMATCH(client.getNick()));
				if (args[2][0] == '-')
					removeOperator(ClientToUpd);
				sendMessage((prefixCompose(client) + "MODE " + client.getNick() + " -o\n"), client.getSockFd());
//				:rch!rch@10.21.34.86 MODE rch -o
				break;
			case 'i':
				if (args[2][0] == '-')
					ClientToUpd->setInvisibleStatus(false);
				else
					ClientToUpd->setInvisibleStatus(true);
				sendMessage((prefixCompose(client) + "MODE " + client.getNick() + " " + (args[2][0] == '+' ? "+" : "-") + "i\n"), client.getSockFd());
				break;
		}
	}
	/* make string for RPL */
	std::string modes = "[+";
	if (isServerOperator(ClientToUpd))
		modes += 'o';
	if (ClientToUpd->getInvisibleStatus())
		modes += 'i';
	modes += "]";
	sendMessage(RPL_UMODEIS(ClientToUpd->getNick(), modes), client.getSockFd());
	if (ClientToUpd->getNick() != client.getNick())
		sendMessage(RPL_UMODEIS(ClientToUpd->getNick(), modes), ClientToUpd->getSockFd());
}

bool Server::isServerOperator(Client *client) {
	std::set<Client *>::iterator it = _operators.begin();
	std::set<Client *>::iterator ite = _operators.end();
	for (; it != ite; it++) {
		if (*it == client)
			return true;
	}
	return false;
}
