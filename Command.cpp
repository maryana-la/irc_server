#include "Server.hpp"
void sendmotd(int sockFd){

	sendMessage(":SERV 001 Hello :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu", sockFd );
	sendMessage(":SERV " + RPL_MOTDSTART((std::string)"SERV"), sockFd);
	sendMessage(":SERV " + RPL_MOTD((std::string)"MOTD is here!!!"), sockFd);
	sendMessage(":SERV " + (std::string)RPL_ENDOFMOTD(), sockFd);
	
}
void Server::passExec(Client &client, std::vector<std::string> &args) {

    /* check amount of args provided */
    if (args.size() != 2) {
		std::string comm = "PASS";
		throw ERR_NEEDMOREPARAMS(comm); //todo maybe kill client
    }

    /* check is registered */
    if (client.getRegisterStatus())
        throw ERR_ALREADYREGISTRED();

    /* if  password is correct */
    if (args[1] == _pass) {
        client.setPassStatus();
        sendMessage("password correct\n", client.getSockFd());
    } else { /* if password is not correct */
        sendMessage("password is not correct\n", client.getSockFd());
        return; // todo maybe kill client?
    }

    /* check is USER & NICK are already filled */
    if (client.checkUserStatus() && !client.getNick().empty()) {
		client.setRegisterStatus();
		sendMessage(":SERV" + RPL_WELCOME(client.getNick()), client.getSockFd());
		sendMessage("client registered\n", client.getSockFd());
		sendmotd(client.getSockFd());
		
	}
}

void Server::nickExec(Client &client, std::vector<std::string> &args) {

	/* check amount of args provided */
	if (args.size() < 2 || args.size() > 3) {
		throw ERR_NONICKNAMEGIVEN();
	}

	/* check if nickname is a valid string */
	if (args[1].find_first_not_of(NICK_VALIDSET) != std::string::npos || args[1].size() > 9)
		throw ERR_ERRONEUSNICKNAME(args[1]);

	/* check if such nick already exists */
	std::vector<Client *>::iterator it = _clients.begin();
	std::vector<Client *>::iterator ite = _clients.end();
	for (; it != ite; it++) {
		if (args[1] == (*it)->getNick() && !client.getRegisterStatus())
			throw ERR_NICKCOLLISION(args[1]);
		else if (args[1] == (*it)->getNick() && client.getRegisterStatus())
			throw ERR_NICKNAMEINUSE(args[1]);
	}

	/* set nickname */
	client.setNick(args[1]);

	/* check if USER & PASS commands are already done succesfully */
	if (client.checkUserStatus() && client.getPassStatus()) {
		client.setRegisterStatus();
		sendMessage("client registered\n", client.getSockFd());
		sendMessage(":SERV" + RPL_WELCOME(client.getNick()), client.getSockFd());
		sendmotd(client.getSockFd());
	}
}


void Server::userExec(Client &client, std::vector<std::string> &args) {

	/* check if number of args is ok */
	if (args.size() != 5) {
		std::string comm = "USER";
		throw ERR_NEEDMOREPARAMS(comm);
	}

	/* check if already registered */
	if (client.getRegisterStatus())
		throw ERR_ALREADYREGISTRED();

	//todo check if stings are valid characters

	client.setUserName(args[1]);
	client.setHostName(args[2]);
	client.setServerName(args[3]);
	client.setRealName(args[4]);

	/* check if NICK & PASS commands are already done succesfully */
	if (!client.getNick().empty() && client.getPassStatus()) {
		client.setRegisterStatus();
		sendMessage(":SERV" + RPL_WELCOME(client.getNick()), client.getSockFd());
		sendMessage("client registered\n", client.getSockFd());
		sendmotd(client.getSockFd());
	}
}


void Server::joinExec(Client &client, std::vector<std::string> &args) {
	/* check if number of args is ok */
	if (args.size() < 2 || args.size() > 3) {
		std::string comm = "JOIN";
		throw ERR_NEEDMOREPARAMS(comm);
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
							throw ERR_BADCHANNELKEY(channels[i]);
					}
					(*it)->addUser(client);
					sendMessage("user is added to channel\n", client.getSockFd());
					break;
				}
			}

			/* if channel is not found */
			if (it == ite) {
				if (_channels.size() == _maxNumberOfChannels)
					throw ERR_TOOMANYCHANNELS(channels[i]);

				/* create new Channel and set attributes */
				Channel *tmp;
				/* check if key was provided */
				if (keys.size() > i)
					tmp = new Channel(channels[i], keys[i], client);
				else
					tmp = new Channel(channels[i], client);
				_channels.push_back(tmp);
				std::string msg = "channel " + channels[i] + " created, admin " + client.getNick() + "\n";
				sendMessage(msg, client.getSockFd());
				topicShort(client, channels[i]);
			}
		}
	}
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

void Server::topicShort(Client &client, const std::string& channelName) {
	std::vector<std::string> forTopic;
	forTopic.push_back("TOPIC");
	forTopic.push_back(channelName);
	topicExec(client, forTopic);
}


void Server::topicExec(Client &client, std::vector<std::string> &args) {
	/* check number of args */
	if (args.size() < 2 || args.size() > 3)
		throw ERR_NEEDMOREPARAMS(args[0]);

	/* just print topic */
	if (args.size() == 2) {
		Channel* tmp = findChannel(args[1]);
		if (tmp == NULL)
			throw "Topic: No such channel\n";
		if (tmp->getTopic().empty())
			sendMessage (RPL_NOTOPIC(tmp->getChannelName()), client.getSockFd());
		else
			sendMessage(tmp->getTopic(), client.getSockFd());
	}
}



void Server::privmsgExec(Client &client, std::vector<std::string> &args) {

	
	
	if(args[1].empty())
		throw ERR_NORECIPIENT((std::string)"PIVMSG");
	if(args[2].empty())
		throw ERR_NOTEXTTOSEND();

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
				throw ERR_CANNOTSENDTOCHAN((*it));
			return;
		}

		Client *clientDest = findClient(*it);
		if (clientDest){
			sendMessage((":" + client.getNick() + " PRIVMSG " + (*it) + " :" + message + "\r\n"), clientDest->getSockFd());
			return;
		}
		else
			throw ERR_NOSUCHNICK(args[1]);
	}
	


}

void Server::modeExec(Client &client, std::vector<std::string> &args){


}

