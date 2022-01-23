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
			if (!(args[0] == "PASS" || args[0] == "pass" || args[0] == "USER" || args[0] == "user" || args[0] == "NICK" || args[0] == "nick" || args[0] == "PART" || args[0] == "part" || args[0] == "KICK" || args[0] == "kick")) {
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
			else if (args[0] == "KICK" || args[0] == "kick")
				kickExec(*client, args);
			else if (args[0] == "MODE" || args[0] == "mode")
				modeExec(*client, args);
//			else if (args[0] == "PONG" || args[0] == "pong")
//				pongExec(*client, args);
			else if (args[0] == "NAMES" || args[0] == "names")
				namesExec(*client, args);
			else if (args[0] == "OPER" || args[0] == "oper")
				operExec(*client, args);
			else if (args[0] == "QUIT" || args[0] == "quit")
				quitExec(*client, args);
			else if (args[0] == "KILL" || args[0] == "kill")
				killExec(*client, args);



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

	//todo send announcment to everybody in channel about new user
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
					if ((*it)->getKeyStatus()) {
						if (keys.size() <= i || (*it)->getKey() != keys[i])
							throw static_cast<std::string>(ERR_BADCHANNELKEY(client.getNick(), channels[i]));
					}
					(*it)->addUser(client);
					standartReply(client, (*it), "JOIN");
					sendTopic(client, channels[i]);
					sendUsers(client, *(*it));
//					(*it)->receiveMsgOfAllChannelUsers(client, (*it));
//					sendMessage(RPL_ENDOFNAMES(client.getNick(), (*it)->getChannelName()), client.getSockFd());
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
					tmp = new Channel(channels[i], keys[i], client, getHost());
				else
					tmp = new Channel(channels[i], client, getHost());
				_channels.push_back(tmp);
				standartReply(client, tmp, "JOIN");
				sendTopic(client, channels[i]);
				sendUsers(client, *tmp);
//				sendMessage(RPL_ENDOFNAMES(client.getNick(), tmp->getChannelName()), client.getSockFd());
//				delete tmp;
			}
//			std::vector<std::string> forNames;
//			forNames.push_back("names");
//			forNames.push_back((*chIt));
//			namesExec(client, forNames);
		}
	}
}

void Server::sendUsers(Client &client, Channel &channel) {
	sendMessage(RPL_NAMREPLY(client.getNick(), channel.getChannelName(),
							 channel.sendUserList(channel.isChannelUser(&client))), client.getSockFd());
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


			channelDest->sendMsgToChan(":" + client.getNick() + " PRIVMSG " + (*it) + " :" + message + "\r\n", &client);


		}
		else{
			Client *clientDest = findClient(*it);
			if (clientDest){
				sendMessage((":" + client.getNick() + " PRIVMSG " + (*it) + " :" + message + "\r\n"), clientDest->getSockFd());
				return;
			}
			else
				throw static_cast<std::string>(ERR_NOSUCHNICK(args[1]));
		}

		
	}
}

void Server::pingExec(Client &client, std::vector<std::string> &args){
	sendMessage(":IRC PONG " + args[1] + "\r\n", client.getSockFd() );
}

void Server::pongExec(Client &client, std::vector<std::string> &args){
	sendMessage(":IRC PING " + args[1] + "\r\n", client.getSockFd() );
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
		leaveChannel(client, channel);
	}
}

void Server::kickExec (Client &client, std::vector<std::string> &args) {
	if(args.size() < 3)
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), args[0]));

	std::vector<std::string> chans = split(args[1],",");
	std::vector<std::string>::iterator itCh = chans.begin();
	std::vector<std::string>::iterator iteCh = chans.end();

	std::vector<std::string> users = split(args[2],",");


	for(; itCh !=iteCh; itCh++){
		Channel *channel = findChannel(*itCh);
		if(!channel)
			throw static_cast<std::string>(ERR_NOSUCHCHANNEL(client.getNick(),(channel->getChannelName())));

		if(!channel->isChannelUser(&client))
			throw static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));

		if(!channel->isOperator(&client))
			throw static_cast<std::string>(ERR_CHANOPRIVSNEEDED(client.getNick(), channel->getChannelName()));

		std::vector<std::string>::iterator itUsers = users.begin();
		std::vector<std::string>::iterator iteUsers = users.end();
		for(; itUsers !=iteUsers; itUsers++){
			Client *user = findClient(*itUsers);
			if(!user)
				throw  static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));

			if(!channel->isChannelUser(user))
				throw static_cast<std::string>(ERR_NOTONCHANNEL(client.getNick(), channel->getChannelName()));
			if(&client == user)
				throw ("Cant kick myself, use PART\n");

//todo replace with leaveChannel function
			channel->deleteUser(*user);
			channel->deleteOperator(*user);

			if(!channel->getNumUsers()){ //remove channel if no users
				std::vector<Channel *>::iterator itChLast = _channels.begin();
				std::vector<Channel *>::iterator iteChLast = _channels.end();
				for (; itChLast != iteChLast; itChLast++) {
					if ((*itChLast)->getChannelName() == channel->getChannelName())
						_channels.erase(itChLast);
				}
			}
		}
	}
}