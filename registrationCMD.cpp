#include "Server.hpp"

void sendmotd(Client &client) {
	sendMessage(RPL_MOTDSTART(client.getNick()), client.getSockFd());
	sendMessage(RPL_MOTD(client.getNick(), (std::string)"comment"), client.getSockFd());
	sendMessage(RPL_ENDOFMOTD(client.getNick()), client.getSockFd());

}

void Server::passExec(Client &client, std::vector<std::string> &args) {

	/* check amount of args provided */
	if (args.size() != 2) {
		std::string comm = "PASS";
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), comm));
	}

	/* check is registered */
	if (client.getRegisterStatus())
		throw static_cast<std::string>(ERR_ALREADYREGISTRED(client.getNick()));

	/* if  password is correct */
	if (args[1] == _password) {
		client.setPassStatus();
	} else /* if password is not correct */
		forceQuit(client);

	/* check is USER & NICK are already filled */
	if (client.checkUserStatus() && !client.getNick().empty()) {
		client.setRegisterStatus();
		sendMessage(RPL_WELCOME(client.getNick()), client.getSockFd());
		sendmotd(client);
	}
}

void Server::nickExec(Client &client, std::vector<std::string> &args) {

	/* check amount of args provided */
	if (args.size() < 2 || args.size() > 3) {
		throw static_cast<std::string>(ERR_NONICKNAMEGIVEN);
	}

	/* check if nickname is a valid string */
	if (args[1].find_first_not_of(NICK_VALIDSET) != std::string::npos || args[1].size() > 9)
		throw static_cast<std::string>(ERR_ERRONEUSNICKNAME(args[1]));

	/* check if such nick already exists */
	std::vector<Client *>::iterator it = _users.begin();
	std::vector<Client *>::iterator ite = _users.end();
	for (; it != ite; it++) {
		if (args[1] == (*it)->getNick() && !client.getRegisterStatus())
			throw static_cast<std::string>(ERR_NICKCOLLISION(args[1]));
		else if (args[1] == (*it)->getNick() && client.getRegisterStatus())
			throw static_cast<std::string>(ERR_NICKNAMEINUSE(args[1]));
	}

	/* set nickname */
	client.setNick(args[1]);

	/* check if USER & PASS commands are already done succesfully */
	if (client.checkUserStatus() && client.getPassStatus()) {
		client.setRegisterStatus();
		sendMessage(RPL_WELCOME(client.getNick()), client.getSockFd());
		sendmotd(client);
	}
}


void Server::userExec(Client &client, std::vector<std::string> &args) {

	/* check if number of args is ok */
	if (args.size() != 5) {
		std::string comm = "USER";
		throw static_cast<std::string>(ERR_NEEDMOREPARAMS(client.getNick(), comm));
	}

	/* check if already registered */
	if (client.getRegisterStatus())
		throw static_cast<std::string>(ERR_ALREADYREGISTRED(client.getNick()));

	client.setUserName(args[1]);
	client.setHostName(args[2]);
	client.setServerName(args[3]);
	client.setRealName(args[4]);

	/* check if NICK & PASS commands are already done succesfully */
	if (!client.getNick().empty() && client.getPassStatus()) {
		client.setRegisterStatus();
		sendMessage(RPL_WELCOME(client.getNick()), client.getSockFd());
		sendmotd(client);
	}
}
