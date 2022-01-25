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

	for (unsigned int i = 0; chIt != chIte; chIt++, i++) {
		if (!checkValidChannelName(channels[i]))
			throw static_cast<std::string>(ERR_NOSUCHCHANNEL(client.getNick(), channels[i]));
		else {
			std::vector<Channel *>::iterator it = _channels.begin();
			std::vector<Channel *>::iterator ite = _channels.end();

			/* check if channel already exists */
			for (; it != ite; it++) {
				/* if channel found */
				if ((*it)->getChannelName() == channels[i]) {
					if ((*it)->getKeyStatus()) {
						if (keys.size() <= i || (*it)->getKey() != keys[i])
							throw static_cast<std::string>(ERR_BADCHANNELKEY(client.getNick(), channels[i]));
					}
					(*it)->addUser(client);
					standartReply(client, (*it), "JOIN", (*it)->getChannelName());
					sendTopic(client, channels[i]);
					sendUsers(client, *(*it));
					break;
				}
			}

			/* if channel is not found */
			if (it == ite) {
				if (static_cast<int>(_channels.size()) == _maxNumberOfChannels)
					throw static_cast<std::string>(ERR_TOOMANYCHANNELS(client.getNick(),channels[i]));

				/* create new Channel and set attributes */
				Channel *tmp;
				/* check if key was provided */
				if (keys.size() > i)
					tmp = new Channel(channels[i], keys[i], client, getHost());
				else
					tmp = new Channel(channels[i], client, getHost());
				_channels.push_back(tmp);
				standartReply(client, tmp, "JOIN", tmp->getChannelName());
				sendTopic(client, channels[i]);
				sendUsers(client, *tmp);
			}
		}
	}
}

void Server::topicExec(Client &client, std::vector<std::string> &args) {
	/* check number of args */
	if (args.size() < 2 || args.size() > 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));

	/* check if channel exists */
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
		standartReply(client, tmp, "TOPIC " + tmp->getChannelName(), tmp->getTopic());
	}
}

void Server::partExec (Client &client, std::vector<std::string> &args) {
	if(args.size() < 2)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));

	std::vector<std::string> dests = split(args[1],",");
	std::vector<std::string>::iterator it = dests.begin();
	std::vector<std::string>::iterator ite = dests.end();

	for(; it !=ite; it++){
		Channel *channel = findChannel(*it);
		if(!channel)
			throw static_cast<std::string>(ERR_NOSUCHCHANNEL(client.getNick(),(*it)));
		if(!channel->isChannelUser(&client))
			throw static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));
		standartReply(client, channel, "PART", channel->getChannelName());
		leaveChannel(client, channel);
	}
}

void Server::kickExec (Client &client, std::vector<std::string> &args) {
	if(args.size() < 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
	/* get channels */
	std::vector<std::string> chans = split(args[1],",");
	std::vector<std::string>::iterator itCh = chans.begin();
	std::vector<std::string>::iterator iteCh = chans.end();

	/* get users to kick */
	std::vector<std::string> users = split(args[2],",");

	for(; itCh !=iteCh; itCh++){
		Channel *channel = findChannel(*itCh);
		/* check if channel exists and requester can kick */
		if(!channel)
			throw static_cast<std::string>(ERR_NOSUCHCHANNEL(client.getNick(),(channel->getChannelName())));

		if(!channel->isChannelUser(&client))
			throw static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));

		if(!channel->isOperator(&client))
			throw static_cast<std::string>(ERR_CHANOPRIVSNEEDED(client.getNick(), channel->getChannelName()));

		std::vector<std::string>::iterator itUsers = users.begin();
		std::vector<std::string>::iterator iteUsers = users.end();
		/* find users to kick in the current channels */
		for(; itUsers !=iteUsers; itUsers++){
			Client *user = findClient(*itUsers);
			if(!user)
				throw  static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));
			if(!channel->isChannelUser(user))
				throw static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));
//			if(&client == user)
//				throw ("Cant kick myself, use PART\n"); //todo!!! delete or kick yourself

			/* announce to group and kick */
			std::string reply;
			if (args.size() > 3)
				reply = channel->getChannelName() + " " + user->getNick() + " :" + args[3];
			else
				reply = channel->getChannelName() + " " + user->getNick();
			standartReply(client, channel, "KICK", reply);
			leaveChannel(*user, channel);
		}
	}
}
