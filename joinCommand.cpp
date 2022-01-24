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
					if ((*it)->getKeyStatus()) {
						if (keys.size() <= i || (*it)->getKey() != keys[i])
							throw static_cast<std::string>(ERR_BADCHANNELKEY(client.getNick(), channels[i]));
					}
					(*it)->addUser(client);
					standartReply(client, (*it), "JOIN", (*it)->getChannelName());
					sendTopic(client, channels[i]);
					sendUsers(client, *(*it));
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
				standartReply(client, tmp, "JOIN", tmp->getChannelName());
				sendTopic(client, channels[i]);
				sendUsers(client, *tmp);
//				sendMessage(RPL_ENDOFNAMES(client.getNick(), tmp->getChannelName()), client.getSockFd());
			}
		}
	}
}
