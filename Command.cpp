#include "Server.hpp"


void Server::parser(Client *client, std::string msg) {


	std::vector<std::string> common;
	//	msg.erase(std::remove(msg.begin(), msg.end(), ':'), msg.end());
	common = split(msg, "\n\r");

	/* find command and execute */

	for (int i = 0; i < common.size(); i++)
	{
		try
		{
			std::vector<std::string> args = split_args(common[i]);
			/* if client is not registered yet */
			if (!(args[0] == "PASS" || args[0] == "pass" || args[0] == "USER" || args[0] == "user" || args[0] == "NICK" || args[0] == "nick" || args[0] == "PART" || args[0] == "part")) {
				if (!client->getRegisterStatus())
					throw static_cast<std::string>(ERR_NOTREGISTERED);
			}

			// todo replace with switch case
			if (args[0] == "PASS" || args[0] == "pass")
				passExec(*client, args);
			else if (args[0] == "USER" || args[0] == "user")
				userExec(*client, args);
			else if (args[0] == "NICK" || args[0] == "nick")
				nickExec(*client, args);
			else if (args[0] == "JOIN" || args[0] == "join")
				joinExec(*client, args);
			else if (args[0] == "LIST" || args[0] == "list")
				listExec(*client, args);
			else if (args[0] == "PRIVMSG" || args[0] == "privmsg")
				privmsgExec(*client, args);
			else if (args[0] == "PING" || args[0] == "ping")
				pingExec(*client, args);
			else if (args[0] == "TOPIC" || args[0] == "topic")
				topicExec(*client, args);
			else if (args[0] == "PART" || args[0] == "part")
				partExec(*client, args);



			//todo clear memory for args in the end
			args.clear();
		}
		catch (const char *msg) {
			std::cout << msg << " char\n";
		}
		catch (std::string &msg) {
			sendMessage(msg, client->getSockFd());
			std::cout << msg << " string\n"; //cout to server
		}
		catch (std::exception &e) {
			sendMessage(e.what(), client->getSockFd());
			std::cout << e.what() << "\n"; //cout to server
			std::cout << msg << " std::exception\n"; //cout to server
		}
		catch (...) {
			std::cout << " catch all\n";
		}
	}
}





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
	}
}

void Server::privmsgExec(Client &client, std::vector<std::string> &args) {

	
	
	if(args[1].empty())
		throw static_cast<std::string>(ERR_NORECIPIENT(client.getNick(), args[0]));
	if(args[2].empty())
		throw static_cast<std::string>(ERR_NOTEXTTOSEND(client.getNick()));

	std::string message;
	for(unsigned int i=2; i<args.size(); i++){
		message += args[i] + " ";//последний аргумент можен содержать пробелы, поэтому объединяем в одно сообщение
	}
	
	std::vector<std::string> dests = split(args[1],",");
	std::vector<std::string>::iterator it = dests.begin();
	std::vector<std::string>::iterator ite = dests.end();
	
	for(; it !=ite; it++){

		if((*it)[0] == '#'){
			Channel *channelDest = findChannel((*it));
			if (!channelDest)
				throw static_cast<std::string>(ERR_CANNOTSENDTOCHAN(client.getNick(),(*it)));

			if(!channelDest->isChannelUser(&client))//todo протестировать проверку что пользователь состоит в канале
				throw static_cast<std::string>(ERR_CANNOTSENDTOCHAN(client.getNick(), channelDest->getChannelName()));


			channelDest->sendMsgToChan(":" + client.getNick() + " PRIVMSG " + (*it) + " :" + message + "\r\n");


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
			case 't':
				ChanToUpd->setTopicOperOnly(isPlus);
				break;
			case 'i':
				ChanToUpd->setInviteOnlyFlag(isPlus);
				break;
			case 'l':
				if (isPlus) {
					if (args.size() != 4)
						throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));
					ChanToUpd->setMaxUsers(strtol(args[3].c_str(), NULL, 10));
				}
				break;
			case 'k':
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

void Server::partExec (Client &client, std::vector<std::string> &args) {
	if(args.size() != 2)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));

	std::vector<std::string> dests = split(args[1],",");
	std::vector<std::string>::iterator it = dests.begin();
	std::vector<std::string>::iterator ite = dests.end();

	for(; it !=ite; it++){
		Channel *channel = findChannel(*it);
		if(!channel)
			throw static_cast<std::string>(ERR_NOSUCHCHANNEL(client.getNick(),(*it)));

		if(!channel->isChannelUser(&client))//todo протестировать проверку что пользователь состоит в канале
			throw static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));

		channel->deleteOperator(client);
		channel->deleteUser(client);
	}
}
