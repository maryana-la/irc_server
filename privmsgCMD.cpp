#include "Server.hpp"

void Server::privmsgExec(Client &client, std::vector<std::string> &args) {

	if(args[1].empty())
		throw static_cast<std::string>(ERR_NORECIPIENT(client.getNick(), args[0]));
	if(args[2].empty())
		throw static_cast<std::string>(ERR_NOTEXTTOSEND(client.getNick()));

	/* split recipients by , */
	std::vector<std::string> dests = split(args[1],",");
	std::vector<std::string>::iterator it = dests.begin();
	std::vector<std::string>::iterator ite = dests.end();


	for(; it != ite; it++) {
		if((*it).find_first_of("&#+!") == 0) {  //if channel name
			Channel *channelDest = findChannel((*it));
			if (!channelDest)
				throw static_cast<std::string>(ERR_CANNOTSENDTOCHAN(client.getNick(),(*it)));
			/* check if client is in channel */
			if(!channelDest->isChannelUser(&client))
				throw static_cast<std::string>(ERR_CANNOTSENDTOCHAN(client.getNick(), channelDest->getChannelName()));

			channelDest->sendMsgToChan(prefixCompose(client) + " PRIVMSG " + (*it) + " :" + args[2] + "\n", &client, false);
		}
		else {  //if user nick
			Client *clientDest = findClient(*it);
			if (clientDest)
				sendMessage((":" + client.getNick() + "!" + client.getUsername() + "@" + client.getHost() + " PRIVMSG " + (*it) + " :" + args[2] + "\n"), clientDest->getSockFd());
			else
				throw static_cast<std::string>(ERR_NOSUCHNICK(*it));
		}
	}
}
