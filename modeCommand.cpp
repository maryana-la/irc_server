#include "Server.hpp"


void Server::modeExec(Client &client, std::vector<std::string> &args) {
	/* check number of args */
	if (args.size() < 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
	/* check number if +- in flag argument */
	if (args[2].find_first_of("+-" != 0))
		throw static_cast<std::string>(ERR_UNKNOWNMODE(client.getNick(), args[2][0]));


	/* if channel */
	if (args[1].find_first_of("&#+!") == 0) {
		if (args[2].find_first_not_of("+-oitlk") != std::string::npos)
			throw static_cast<std::string>(ERR_UMODEUNKNOWNFLAG(client.getNick(), args[1]));
		setChannelModes(client, args);
	}


		/* if user */
	else {

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
					throw static_cast<std::string>(ERR_USERSDONTMATCH(client.getNick(), ChanToUpd->getChannelName()));
				/* add/delete new operator of the channel */
				if (isPlus)
					ChanToUpd->addOperator(*ClientToUpd);
				else
					ChanToUpd->deleteOperator(*ClientToUpd);
				break;
			case 't': /* topic can be set only by operator */
				ChanToUpd->setTopicOperOnly(isPlus);
				break;
			case 'i':
				ChanToUpd->setInviteOnlyFlag(isPlus);
				break;
			case 'l': /* set limit of users in th channel */
				if (isPlus) {
					if (args.size() != 4)
						throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
					ChanToUpd->setMaxUsers(strtol(args[3].c_str(), NULL, 10));
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
	if (ChanToUpd->getInviteOnlyFlag())
		modes += "i";
	if (ChanToUpd->getKeyStatus())
		modes += "k";
	if (ChanToUpd->getMaxUsers())
		modes += "l(" + intToString(ChanToUpd->getMaxUsers()) + ")";
	modes += "]";
	sendMessage(RPL_CHANNELMODEIS(client.getNick(), ChanToUpd->getChannelName(), modes), client.getSockFd());
}

//MODE #AS +t
//:IRC.1 324 QW #AS [+nt]
//:QW!QW@127.0.0.1 MODE #AS +t
//
//MODE ZX +i
//:IRC.1 221 ZX [+i]