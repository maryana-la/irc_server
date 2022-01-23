#include "Server.hpp"


void Server::modeExec(Client &client, std::vector<std::string> &args) {
	/* check number of args */
	if (args.size() < 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
	/* check number if +- in flag argument */
//	size_t tmp = args[2].find_first_of("+-");
	if (args[2][0] != '+' && args[2][0] != '-')
		throw static_cast<std::string>(ERR_UNKNOWNMODE(client.getNick(), args[2][0]));


	/* if channel */
	if (args[1].find_first_of("&#+!") == 0) {
		if (args[2].find_first_not_of("+-oitlk") != std::string::npos)
			throw static_cast<std::string>(ERR_UMODEUNKNOWNFLAG(client.getNick(), args[1]));
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
				break;
			case 't': /* topic can be set only by operator */
				ChanToUpd->setTopicOperOnly(isPlus);
				break;
//			case 'i':
//				ChanToUpd->setInviteOnlyFlag(isPlus);
//				break;
			case 'l': /* set limit of users in th channel */
				if (isPlus) {
					if (args.size() != 4)
						throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
					ChanToUpd->setMaxUsers(strtol(args[3].c_str(), NULL, 10));
					ChanToUpd->setMaxUsersFlag(true);
				}
				else
					ChanToUpd->setMaxUsersFlag(false);
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
				break;
		}
	}
	/* make string for RPL */
	std::string modes = "[+n";
	if (ChanToUpd->getTopicOperatorsOnly())
		modes += "t";
//	if (ChanToUpd->getInviteOnlyFlag())
//		modes += "i";
	if (ChanToUpd->getKeyStatus())
		modes += "k";
	if (ChanToUpd->getMaxUsersFlag())
		modes += "l(" + intToString(ChanToUpd->getMaxUsers()) + ")";
	modes += "]";
	sendMessage(RPL_CHANNELMODEIS(client.getNick(), ChanToUpd->getChannelName(), modes), client.getSockFd());
}

void Server::setUserModes(Client &client, std::vector<std::string> &args) {
	Client *ClientToUpd = findClient(args[1]);
	if (!ClientToUpd)
		throw static_cast<std::string>(ERR_NOSUCHNICK(args[1]));
	std::string::iterator it = args[2].begin() + 1;
	for (; it != args[2].end(); it++) {
		switch (*it) {
			case 'o':
				if (!isServerOperator(&client))
					throw static_cast<std::string>(ERR_USERSDONTMATCH(client.getNick()));
				if (args[2][0] == '-')
					_operators.erase(ClientToUpd);
				break;
			case 'i':
				if (args[2][0] == '-')
					ClientToUpd->setInvisibleStatus(false);
				else
					ClientToUpd->setInvisibleStatus(true);
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



//MODE #AS +t
//:IRC.1 324 QW #AS [+nt]
//:QW!QW@127.0.0.1 MODE #AS +t
//
//MODE ZX +i
//:IRC.1 221 ZX [+i]