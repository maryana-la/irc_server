#include "Server.hpp"

Server::Server(const std::string *host, const std::string &port, const std::string &password)
		: _socketFd(-1), _host(host), _port(port), _password(password), _maxNumberOfChannels(30) {
	_operator_login.insert(std::make_pair("jjacquel", "ircserv"));
	_operator_login.insert(std::make_pair("rchelsea", "ircserv"));
}

void Server::begin() {
	int ptr = 1;
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

	if ((getaddrinfo(NULL, _port.c_str(), &hints, &res)) != 0)
		throw std::runtime_error("Port/address errorMain");
	if ((_socketFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
		throw std::runtime_error("Connection errorMain");
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &ptr, sizeof(int)) == -1)
		throw std::runtime_error("Connection errorMain");
	if (bind(_socketFd, res->ai_addr, res->ai_addrlen) != 0){
		close(_socketFd);
		throw std::runtime_error("Connection errorMain");
	}
	freeaddrinfo(res);
	if (listen(_socketFd, 32) == -1)
		throw std::runtime_error("listen errorMain");
	pollfd pfd = {_socketFd, POLLIN, 0};
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl errorMain");
	_fds.push_back(pfd);
	std::vector<pollfd>::iterator it;
	while (true) {
		it = _fds.begin();
		if (poll(&(*it), _fds.size(), -1) == -1) 
			throw std::runtime_error("poll errorMain");
		exec();
	}
}

Server::~Server() {
	_channels.clear();
	_users.clear();
}

Client *Server::findClientbyFd(int fd) {
	std::vector<Client*>::iterator it = _users.begin();
	std::vector<Client*>::iterator ite = _users.end();
	for (; it != ite; it++) {
		if (fd == (*it)->getSockFd()) {
			return *it;
		}
	}
	return NULL;
}

void Server::exec() {
	pollfd nowPollfd;
	for (unsigned int i = 0; i < _fds.size(); i++) {
		nowPollfd = _fds[i];
		if ((nowPollfd.revents & POLLIN) == POLLIN) {
			if (nowPollfd.fd == _socketFd) {
				int clientSocket;
				sockaddr_in clientAddr;
				memset(&clientAddr, 0, sizeof(clientAddr));
				socklen_t socketLen = sizeof(clientAddr);
				clientSocket = accept(_socketFd, (sockaddr *) &clientAddr, &socketLen);
				if (clientSocket == -1) {
					continue;
				}
				pollfd clientPollfd = {clientSocket, POLLIN, 0};
				_fds.push_back(clientPollfd);
				if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1) {
					throw std::runtime_error("fcntl errorMain");
				}
				std::cout << "new fd:" << clientSocket << std::endl;
				Client *user = new Client(clientSocket, clientAddr);
				_users.push_back(user);
			} else {
				try {
					Client *curUser = findClientbyFd(nowPollfd.fd);
					std::string receivedMsg = recvMessage(curUser->getSockFd());
					curUser->messageAppend(receivedMsg);
					if (curUser->getReadCompleteStatus())
						parser(curUser, curUser->getMessage());
				} catch (std::runtime_error &e) {
					std::cout << e.what() << std::endl;
                    close(_fds[i].fd);
                    _fds.erase(_fds.begin() + i);
				}
			}
		}
		if ((nowPollfd.revents & POLLHUP) == POLLHUP) {
			Client *user = findClientbyFd(nowPollfd.fd);
			if (user == NULL)
				continue;
			forceQuit(*user);
		}
	}
}

std::string Server::recvMessage(int fd) {
	char message[512];
	std::string res;
	int recvByte = 0;
	int readed;
	memset(message, '\0', sizeof(message));
	while ((readed = recv(fd, message, sizeof(message), 0)) > 0) {
		message[readed] = 0;
		recvByte += readed;
		res += message;
		if (res.find("\n") != std::string::npos)
			break;
	}
	while (res.find("\r") != std::string::npos)      // Удаляем символ возврата карретки
		res.erase(res.find("\r"), 1);
	if (recvByte <= 0) {
		throw std::runtime_error("fd " + intToString(fd) + " disconnected");
	}
	std::cout << "from fd " << fd << ": " << res;
	return (res);
}

