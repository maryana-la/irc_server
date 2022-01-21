#include "Server.hpp"

void Server::joinExec(Client &client, std::vector<std::string> &args) {
	/* check if number of args is ok */
	if (args.size() < 2 || args.size() > 3) {
		std::string comm = "JOIN";
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), comm));
	}

	/* split channels and keys by ',' */
	std::vector<std::string> channels;
	std::vector<std::string> keys;
	channels = split(args[1], ",");
	if (args.size() == 3)
		keys = split(args[2], ",");

	/* iterate by channels */
	std::vector<std::string>::iterator chIt = channels.begin();
	std::vector<std::string>::iterator chIte = channels.end();

	for (int i = 0; chIt != chIte; chIt++, i++) {
		if (!checkValidChannelName(channels[i]))
			throw static_cast<std::string>(ERR_NOSUCHCHANNEL(client.getNick(), channels[i]));
		else {
			std::vector<Channel *>::iterator it = _channels.begin();
			std::vector<Channel *>::iterator ite = _channels.end();

			/* check if channel already exists */
			for (; it != ite; it++) {
				/* if channel found */
				if ((*it)->getChannelName() == channels[i]) {
					//todo check if invite only/invitation
					if ((*it)->getKeyStatus()) {
						if (keys.size() <= i || (*it)->getKey() != keys[i])
							throw static_cast<std::string>(ERR_BADCHANNELKEY(client.getNick(), channels[i]));
					}
					(*it)->addUser(client);
					sendTopic(client, channels[i]);
					sendUsers(client, *(*it));
					break;
				}
			}

			/* if channel is not found */
			if (it == ite) {
				if (_channels.size() == _maxNumberOfChannels)
					throw static_cast<std::string>(ERR_TOOMANYCHANNELS(client.getNick(),channels[i]));

				/* create new Channel and set attributes */
				Channel *tmp;
				/* check if key was provided */
				if (keys.size() > i)
					tmp = new Channel(channels[i], keys[i], client);
				else
					tmp = new Channel(channels[i], client);
				_channels.push_back(tmp);
				sendTopic(client, channels[i]);
				sendUsers(client, *tmp);
			}
		}
	}
}

void Server::sendUsers(Client &client, Channel &channel) {
	sendMessage(RPL_NAMREPLY(client.getNick(), channel.getChannelName(),
							 channel.sendUserList()), client.getSockFd());
	sendMessage(RPL_ENDOFNAMES(client.getNick(), channel.getChannelName()), client.getSockFd());
}

void Server::listExec(Client &client, std::vector<std::string> &args) {
//todo check private and hidden attributes
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

void Server::sendTopic(Client &client, const std::string& channelName) {
	std::vector<std::string> forTopic;
	forTopic.push_back("TOPIC");
	forTopic.push_back(channelName);
	topicExec(client, forTopic);
}

void Server::topicExec(Client &client, std::vector<std::string> &args) {
	/* check number of args */
	if (args.size() < 2 || args.size() > 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));

	Channel* tmp = findChannel(args[1]);
	if (tmp == NULL)
		throw static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), args[1]));

	if (args.size() == 2) {  /* just print topic */
		if (tmp->getTopic().empty())
			sendMessage (RPL_NOTOPIC(client.getNick(),tmp->getChannelName()), client.getSockFd());
		else
			sendMessage(RPL_TOPIC(client.getNick(),tmp->getChannelName(), tmp->getTopic()), client.getSockFd());
	}
	else {  /* set new topic */
		if (!tmp->isChannelUser(&client))
			throw static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(),tmp->getChannelName()));

		/* if topic can be changed only by operators and current user is not operator */
		if (tmp->getTopicOperatorsOnly() && !tmp->isOperator(&client))
			throw static_cast<std::string>(ERR_CHANOPRIVSNEEDED(client.getNick(), tmp->getChannelName()));

		tmp->setTopic(args[2]);
	}
}

void Server::privmsgExec(Client &client, std::vector<std::string> &args) {

	
	
	if(args[1].empty())
		throw static_cast<std::string>(ERR_NORECIPIENT(client.getNick(), args[0]));
	if(args[2].empty())
		throw static_cast<std::string>(ERR_NOTEXTTOSEND(client.getNick()));

	std::string message;
	for(unsigned int i=2; i<args.size(); i++){
		message += args[i];
	}
	
	std::vector<std::string> dests = split(args[1],",");
	std::vector<std::string>::iterator it = dests.begin();
	std::vector<std::string>::iterator ite = dests.end();
	
	for(; it !=ite; it++){

		if((*it)[0] == '#'){
			Channel *channelDest = findChannel((*it));
			if (channelDest){
				channelDest->sendMsgToChan(":" + client.getNick() + " PRIVMSG " + (*it) + " :" + message + "\r\n");
			}
			else
				throw static_cast<std::string>(ERR_CANNOTSENDTOCHAN(client.getNick(),(*it)));
			return;
		}

		Client *clientDest = findClient(*it);
		if (clientDest){
			sendMessage((":" + client.getNick() + " PRIVMSG " + (*it) + " :" + message + "\r\n"), clientDest->getSockFd());
			return;
		}
		else
			throw static_cast<std::string>(ERR_NOSUCHNICK(args[1]));
	}
}

void Server::pingExec(Client &client, std::vector<std::string> &args){
	sendMessage(":SERVNAME PONG " + args[1], client.getSockFd());
}

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

		setChannelmodes(client, args);

	}


	/* if user */
	else {

	}
}

void Server::setChannelmodes(Client &client, std::vector<std::string> &args) {
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
				/* add new operator of the channel */
				if (isPlus)
					ChanToUpd->addOperator(*ClientToUpd);
				else
					ChanToUpd->deleteOperator(*ClientToUpd);
				break; //todo add reply
			case 't':
				ChanToUpd->setTopicOperOnly(isPlus);
				break;
			case 'i':
				ChanToUpd->setInviteOnlyFlag(isPlus);
				break;
//			case 'l':
//				if (args.size() != 4)
//					throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
//				for (int i = 0; i < args[3].size(); i++) {
//					if (!args[3][i])
//				}
//				ChanToUpd->setMaxUsers();

		}
	}
}









