#include "Server.hpp"

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
		sendMessage("client registered\n", client.getSockFd());
	}
}

void Server::nickExec(Client &client, std::vector<std::string> &args) {

	/* check amount of args provided */
	if (args.size() < 2 || args.size() > 3) {
		throw ERR_NONICKNAMEGIVEN();
	}

	/* check if nickname is a valid string */
	if (args[1].find_first_not_of(VALIDSET) != std::string::npos || args[1].size() > 9)
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
		sendMessage("client registered\n", client.getSockFd());
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

	/* only 1 argument provided */
	if (args.size() == 2) {
		if (channels[0][0] == '#') {
			std::vector<Channel *>::iterator it = _channels.begin();
			std::vector<Channel *>::iterator ite = _channels.end();

			/* check if channel already exists */
			for (; it != ite; it++) {
				/* if channel found */
				if ((*it)->getChannelName() == channels[0]) {
					//todo check if user banned, if invite only, if < maxUsers
					(*it)->addUser(client);
					sendMessage("user added to channel", client.getSockFd());
					break;
				}
			}

			/* if channel is not found */
			if (it == ite) {
				/* create new Channel and set attributes */
				Channel *tmp = new Channel(channels[0], client);
				_channels.push_back(tmp);
				std::string msg = "channel " + channels[0] + " created, admin " + client.getNick() + "\n";
				sendMessage(msg, client.getSockFd());
			}
		}
	}
}





