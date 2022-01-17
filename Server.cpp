#include "Server.hpp"

Server::Server(std::string host, std::string port, std::string pass) : _host(host), _port(port), _pass(pass) {
	std::stringstream stream;
	
	int portCheck = 0;
	stream << port;
	stream >> portCheck;

	if (portCheck < 1 || portCheck > 65535)
		Server::Fatal("The port number is invalid it must be between 1 and 65535 !");
	this->createSocket();
	for (int i = 0; i < 3; i++)
		_id[i] = 1;

	_maxNumberOfChannels = 100;
}

int		Server::getId(int i){
	if (i < 0 || i > 2)
		Server::Fatal("Error: getId");
	if (_id[i] > 32)
		_id[i] = 1;
	_id[i]++;
	return (_id[i]);
}

void	Server::createSocket(void){
	addrinfo	hints;
	addrinfo	*servinfo;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_PASSIVE;
	if (getaddrinfo(_host.c_str(), _port.c_str(), &hints, &servinfo) != 0)
		Server::Fatal("Error: getaddrinfo");

	addrinfo	*p;
	int			sock;
	int			yes = 1;

	for (p = servinfo; p != nullptr; p = p->ai_next) {
		sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sock == -1)
			continue;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			close(sock);
			freeaddrinfo(servinfo);
			Server::Fatal("Error: setsockopt");
		}
		if (bind(sock, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sock);
			continue;
		}
		break;
	}
	freeaddrinfo(servinfo);
	if (p == nullptr)
		Server::Fatal("Error: failed to find address");
	if (listen(sock, MAX_CONNECTION) == -1)
		Server::Fatal("Error: listen");

	_sock = sock;
}

void Server::start(void){
	pollfd	newPollfd = {_sock, POLLIN, 0};
	pollfd	ptrPollfd;

//	_Commander = new Command(this);
	if (fcntl(_sock, F_SETFL, O_NONBLOCK) == -1)
	 	Server::Fatal("Error: poll: fcntl");
	
	std::vector<pollfd>::iterator	iterPoll;
	_pollfds.push_back(newPollfd);



	std::cout << "Server created!" << std::endl;

	while (1)
	{
		iterPoll = _pollfds.begin();
		if (poll(&(*iterPoll), _pollfds.size(), -1) == -1)
			Server::Fatal("Error: poll");
		for (std::vector<pollfd>::iterator itPollfd = _pollfds.begin(); itPollfd != _pollfds.end(); itPollfd++)
		{
			ptrPollfd = *itPollfd;

			if ((ptrPollfd.revents & POLLHUP) == POLLHUP){
				std::vector<Client *>::iterator	itClient = _clients.begin();
        	    advance(itClient, distance(_pollfds.begin(), itPollfd) - 1);

				if (_clients.empty())
					break;
        	    break;
        	}

			if ((ptrPollfd.revents & POLLIN) == POLLIN)
			{
				if (ptrPollfd.fd == _sock)
				{
					std::string str("Entered:\nPASS <password>\nNICK <nickname>\nUSER <username> <flags> <unused> <realname>\n\r");
						if (send(createClient(), str.c_str(), str.length(), 0) == -1)
							Server::Fatal("Error: send");
					break ;
				}
				else
				{
					std::vector<Client *>::iterator	itClient = _clients.begin();
					advance(itClient, distance(_pollfds.begin(), itPollfd) - 1);
					recvMessage(*itClient);

					std::cout << (*itClient)->getNick() << ": " << (*itClient)->getMessage() << "\n";
					std::cout << "parcer called\n";
//					Client *tmp = *itClient;
//					if (isdigit(tmp->getMessage()[0]))
//						send(atoi(tmp->getMessage().c_str()), tmp->getMessage().c_str(), tmp->getMessage().length(), 0);
//					send(tmp->getSockFd(), tmp->getMessage().c_str(), tmp->getMessage().length(), 0);

                    parser((*itClient), (*itClient)->getMessage());

//					_Commander->parse((*itClient), (*itClient)->getMessage());
				}
			}
		}
	}
}

void	Server::recvMessage(Client *client){
	ssize_t		byteRecved;
	char		message[100];

	client->clearMessage();
	memset(message, '\0', sizeof(message));
	while (!std::strstr(message, "\r\n"))
	{
		memset(message, '\0', sizeof(message));
		byteRecved = recv(client->getSockFd(), message, sizeof(message), 0);
		if (byteRecved <= 0)
			break ;
		client->appendMessage(message);
	}
}


int		Server::createClient(void){
	int				client_d = 0;
	sockaddr_in		client_addr;
	socklen_t		s_size;

	memset(&client_addr, 0, sizeof(client_addr));
	s_size = sizeof(client_addr);
	if ((client_d = accept(_sock, (sockaddr *) &client_addr, &s_size)) == -1)
		Server::Fatal("Error: accept");

	pollfd	newPollfd = {client_d, POLLIN, 0};
	_pollfds.push_back(newPollfd);
	if (fcntl(client_d, F_SETFL, O_NONBLOCK) == -1)
	 	Server::Fatal("Error: poll: fcntl");
	std::cout << "New client generated\n";
	Client	*newClient = new Client(client_d, ntohs(client_addr.sin_port), this, inet_ntoa(client_addr.sin_addr));
	_clients.push_back(newClient);

	std::cout << "New client " << newClient->getNick() << "@" << newClient->getHost() << ":" << newClient->getPort() << std::endl;
	return client_d;
}

void Server::Fatal(std::string str){
	std::cerr << str << std::endl;
	exit(1);
}


Client::Client(int sockFd, int port, Server *serv, char *host) : _sockFd(sockFd), _port(port), _host(host){
	_nickname = "";
	_realname = "";
	_awayMessage = "";
    _passValid = false;
	_id = serv->getId(0) + serv->getId(1) + serv->getId(2);
}

void		Client::clearMessage(void){
	_message.clear();
}

void			Client::appendMessage(std::string message)
{
	_message.append(message);
	_message.erase(_message.find_last_not_of("\r\n") + 1);
	_message.append("\n");
}

void Server::parser(Client *client, std::string msg) {


	std::vector<std::string> common;
	common = split(msg, "\n\r");

//    char* tmp;
//    tmp = std::strtok(const_cast<char *>(msg.c_str()), " ");
//
//    while (tmp != NULL) {
//        args.push_back(tmp);
//        tmp = strtok (NULL, " \n");
//    }


    /* find command and execute */

	for (int i = 0; i < common.size(); i++)
	{
		std::vector<std::string> args = split(common[i], " ");
		try
		{
			// todo replace with switch case
			if (args[0] == "PASS" || args[0] == "pass")
				passExec(*client, args);
			else if (args[0] == "USER" || args[0] == "user")
				userExec(*client, args);
			else if (args[0] == "NICK" || args[0] == "nick")
				nickExec(*client, args);
			else if (args[0] == "JOIN" || args[0] == "join")
				joinExec(*client, args);
			else if (args[0] == "PRIVMSG" || args[0] == "privmsg")
				privmsgExec(*client, args);


			//todo clear memory for args in the end
			args.clear();
		}
		catch (char *msg)
		{
			std::cout << msg << " char\n";
		}
		catch (std::string &msg)
		{
			sendMessage(msg, client->getSockFd());
			std::cout << msg << " string\n"; //cout to server
		}
		catch (...)
		{
			std::cout << " catch all\n";
		}
	}
}



void sendMessage(const std::string &msg, int socket_fd) {
    send(socket_fd, msg.c_str(), msg.length(), 0);
}
