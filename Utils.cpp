#include "Server.hpp"


void sendMessage(const std::string &msg, int socket_fd) {
	std::cout << "To fd " << socket_fd << ": \"" << msg;
	send(socket_fd, msg.c_str(), msg.length(), IRC_NOSIGNAL);
}


std:: vector<std::string> split(const std::string& line, const std::string& delimiter) {
	std::vector<std::string> args;
	char* tmp;
	tmp = std::strtok(const_cast<char *>(line.c_str()), const_cast<char *>(delimiter.c_str()));

	while (tmp != NULL) {
		args.push_back(tmp);
		tmp = strtok (NULL, const_cast<char *>(delimiter.c_str()));
	}
	return args;
}


std:: vector<std::string> split_args(const std::string& line) {
	if (line.length() > 510)
		throw "Line is too long\n";

	std::vector<std::string> args;
	size_t i = 0;
	while (i < line.length()) {
		if (line[i] == ' ')
			i++;
		else {
			std::string tmp;
			size_t begin = i;
			if (line[i] == ':') {
				i = line.length();
				begin++;
			}
			else
				i = line.find(' ', i);
			tmp.assign(line, begin, i - begin);
			args.push_back(tmp);
		}
	}
	return args;
}

std::string	intToString(long int num) {
	std::stringstream ss;
	ss << num;
	return(ss.str());
}

//Channels names are strings (beginning with a '&', '#', '+' or '!' character) of length up to 50 characters.
//Channel names are case insensitive. The only restriction on a channel name is that it SHALL NOT contain any spaces,
//a control G (ASCII 7) or a comma (',' which is used as a list item separator by the protocol).
//A colon (':') is used as a delimiter for the channel mask.
// Channels with '&' as prefix are local to the server where they are created.
int checkValidChannelName(const std::string &name) {
	if (name.length() > 50 || name.find_first_of("&#+!") != 0)
		return 0;
	for (unsigned int i = 0; i < name.length(); i++) {
		if (isspace(name[i]) || iscntrl(name[i]) || name[i] == ',')
			return 0;
	}
	return 1;
}

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

void Server::removeOperator(Client *client) { _operators.erase(client); }

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
	msg = ":" + client.getNick() + "!" + client.getUsername() + "@" + getHost() + " ";
	return msg;
}

/* client - who made an action, channel - where action was made */
void Server::standartReply(Client &client, Channel *channel, const std::string &command, const std::string &param) {//todo add : for join before param for join?
	std::string msg;
	msg = prefixCompose(client) + command + " " + param + "\n";
	channel->sendMsgToChan(msg, &client, true);
}


std::string getIP()
{
	const char* google_dns_server = "8.8.8.8";
	int dns_port = 53;

	struct sockaddr_in serv;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	//Socket could not be created
	if(sock < 0)
	{
		std::cout << "Socket errorMain" << std::endl;
	}

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(google_dns_server);
	serv.sin_port = htons(dns_port);

	int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
	if (err < 0)
	{
		std::cout << "Error number: " << errno
				  << ". Error message: " << strerror(errno) << std::endl;
	}

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (struct sockaddr*)&name, &namelen);

	char buffer[80];
	const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
	if(p != NULL)
	{
//		std::cout << "Local IP address is: " << buffer << std::endl;
	}
	else
	{
		std::cout << "Error number: " << errno
				  << ". Error message: " << strerror(errno) << std::endl;
	}

	close(sock);

	std::string ret = buffer;
	return (ret);
}

void errorMain(){
	std::cout << "Argument errorMain, usage: ircserver [host:port_network:password_network] <port> <password>\n";
	exit(1);
}
