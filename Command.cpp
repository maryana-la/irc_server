#include "Server.hpp"

void Server::joinExec(Client &client, std::vector<std::string> &args) {
	/* check if number of args is ok */
	if (args.size() < 2 || args.size() > 3) {
		std::string comm = "JOIN";
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(comm));
	}

	/* if client is not registered yet */
	if (!client.getRegisterStatus())
		return;

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
		if (checkValidChannelName(channels[i])) {
			std::vector<Channel *>::iterator it = _channels.begin();
			std::vector<Channel *>::iterator ite = _channels.end();

			/* check if channel already exists */
			for (; it != ite; it++) {
				/* if channel found */
				if ((*it)->getChannelName() == channels[i]) {
					//todo check if invite only/invitation
					if ((*it)->getKeyStatus()) {
						if (keys.size() <= i || (*it)->getKey() != keys[i])
							throw static_cast<std::string>(ERR_BADCHANNELKEY(channels[i]));
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
					throw static_cast<std::string>(ERR_TOOMANYCHANNELS(channels[i]));

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

	/* if list without arguments */
	if (args.size() == 1) {
		std::vector<Channel*>::iterator it= _channels.begin();
		std::vector<Channel*>::iterator ite= _channels.end();
		for(; it !=ite; it++) {
			std::string msg = (*it)->getChannelName() + " " + (*it)->getTopic() + "\n";
			sendMessage(msg, client.getSockFd());
		}
	}
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
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(args[0]));

	/* just print topic */
	if (args.size() == 2) {
		Channel* tmp = findChannel(args[1]);
		if (tmp == NULL)
			throw static_cast<std::string>("Topic: No such channel\n");
		if (tmp->getTopic().empty())
			sendMessage (RPL_NOTOPIC(tmp->getChannelName()), client.getSockFd());
		else
			sendMessage(RPL_TOPIC(tmp->getChannelName(), tmp->getTopic()), client.getSockFd());
	}

	/* set new topic */
//	ERR_CHANOPRIVSNEEDED
//	442     ERR_NOTONCHANNEL
//	"<channel> :You're not on that channel"
//
//	- Возвращается сервером, как только клиент пытается
//	выполнить команду канала, на котором отсутствует.
}



void Server::privmsgExec(Client &client, std::vector<std::string> &args) {

	
	
	if(args[1].empty())
		throw static_cast<std::string>(ERR_NORECIPIENT((std::string)"PRIVMSG"));
	if(args[2].empty())
		throw static_cast<std::string>(ERR_NOTEXTTOSEND);

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
				throw static_cast<std::string>(ERR_CANNOTSENDTOCHAN((*it)));
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

//	class pass_mismatch : public std::exception
//	{
//		const char *what() const throw()
//		{
//			return ":server 464 pass :Password incorrect\r\n";
//		}
//	};

}

void Server::modeExec(Client &client, std::vector<std::string> &args){


}

