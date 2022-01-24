#include "Server.hpp"

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

			channelDest->sendMsgToChan(":" + client.getNick() + "!" + client.getUsername() + "@" + getHost() + " PRIVMSG " + (*it) + " :" + message + "\n", &client);
		}
		else{
			Client *clientDest = findClient(*it);
			if (clientDest){
				sendMessage((":" + client.getNick() + "!" + client.getUsername() + "@" + getHost() + " PRIVMSG " + (*it) + " :" + message + "\n"), clientDest->getSockFd());
				return;
			}
			else
				throw static_cast<std::string>(ERR_NOSUCHNICK(args[1]));
		}
	}
}
