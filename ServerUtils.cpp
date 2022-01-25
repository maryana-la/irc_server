#include "Server.hpp"


Client *Server::findClient(const std::string &clientNick){
	std::vector<Client*>::iterator it = _users.begin();
	std::vector<Client*>::iterator ite = _users.end();
	for(; it !=ite; it++) {
		if ((*it)->getNick() == clientNick)
			return (*it);
	}
	return NULL;
}

Channel *Server::findChannel(const std::string &channelName){
	std::vector<Channel*>::iterator it = _channels.begin();
	std::vector<Channel*>::iterator ite = _channels.end();
	for(; it !=ite; it++){
		if ((*it)->getChannelName() == channelName)
			return (*it);
	}
	return NULL;
}


void Server::sendTopic(Client &client, const std::string& channelName) {
	std::vector<std::string> forTopic;
	forTopic.push_back("TOPIC");
	forTopic.push_back(channelName);
	topicExec(client, forTopic);
}

void Server::sendUsers(Client &client, Channel &channel) {
	sendMessage(RPL_NAMREPLY(client.getNick(), channel.getChannelName(),
							 channel.sendUserList(channel.isChannelUser(&client))), client.getSockFd());
	sendMessage(RPL_ENDOFNAMES(client.getNick(), channel.getChannelName()), client.getSockFd());
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

void Server::removeClient(Client *user) {
	std::vector<Client *>::iterator it = _users.begin();
	std::vector<Client *>::iterator ite = _users.end();
	for (; it != ite; it++) {
		if (*it == user) {
			_users.erase(it);
			break;
		}
	}
}

void Server::removeOperator(Client *client) {
	_operators.erase(client);
}

void Server::leaveChannel(Client &client, Channel *channel) {
	if (!channel)
		return;
	channel->deleteUser(client);
	channel->deleteOperator(client);
	if(!channel->getNumUsers()){  //remove channel if no users
		std::vector<Channel *>::iterator itCh = _channels.begin();
		std::vector<Channel *>::iterator iteCh = _channels.end();
		for (; itCh != iteCh; itCh++) {
			if ((*itCh)->getChannelName() == channel->getChannelName())
				_channels.erase(itCh);
		}
	}
}

std::string Server::prefixCompose(Client &client) {
	std::string msg;
	msg = ":" + client.getNick() + "!" + client.getUsername() + "@" + client.getHost() + " ";
	return msg;
}

/* client - who made an action, channel - where action was made */
void Server::standartReply(Client &client, Channel *channel, const std::string &command, const std::string &param) {
	std::string msg;
	msg = prefixCompose(client) + command + " " + param + "\n";
	channel->sendMsgToChan(msg, &client, true);
}
