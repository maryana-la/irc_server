#include "Server.hpp"


#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <array>
#include <arpa/inet.h>
#include "Server.hpp"
//#include "Client.hpp"


Server::Server(const std::string &host, const std::string &port, const std::string &password)
		: _socketFd(-1), _host(host), _port(port), _password(password), _maxNumberOfChannels(30) {
	_operator_login.insert(std::make_pair("jjacquel", "ircserv"));
	_operator_login.insert(std::make_pair("rchelsea", "ircserv"));
}

/**
 * создание структуры addrinfo, создание сокета и bind
 */
void Server::init() {
	int newSocketFd;
	int yes = 1;
	struct addrinfo hints, *serverInfo, *rp;

	memset(&hints, 0, sizeof hints); // убедимся, что структура пуста
	hints.ai_family = AF_UNSPEC;     // неважно, IPv4 или IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream-sockets
	hints.ai_flags = AI_PASSIVE;     // заполните мой IP-адрес за меня
//	getaddrinfo("10.21.34.84", _port.c_str(), &hints, &servinfo) != 0)

	if (getaddrinfo(_host != "127.0.0.1" ? this->_host.c_str() : "127.0.0.1", this->_port.c_str(), &hints, &serverInfo) != 0) {
		throw std::runtime_error("getaddrinfo error");
	}
	for (rp = serverInfo; rp != nullptr; rp = rp->ai_next) {
		newSocketFd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (newSocketFd == -1) {
			continue;
		}
		if (setsockopt(newSocketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			throw std::runtime_error("setsockopt error");
		}
		if (bind(newSocketFd, rp->ai_addr, rp->ai_addrlen) == 0) {
			break; // Success
		}
		close(newSocketFd);
	}
	if (rp == nullptr) {
		throw std::runtime_error("bind error");
	}
	freeaddrinfo(serverInfo); /// освобождаем связанный список
	this->_socketFd = newSocketFd;
}

/**
 * listen сокета, создание pollfd структуры для этого сокета,
 * добавление в вектор структур, и главный цикл
 */
void Server::start() {
	if (listen(this->_socketFd, 10) == -1) {
		throw std::runtime_error("listen error");
	}
	pollfd sPollfd = {this->_socketFd, POLLIN, 0};
	if (fcntl(this->_socketFd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("fcntl error");
	}
	this->_fds.push_back(sPollfd);
	std::vector<pollfd>::iterator it;
	while (true) {
		it = this->_fds.begin();
		if (poll(&(*it), this->_fds.size(), -1) == -1) {
			throw std::runtime_error("poll error");
		}
		///после этого нужно что-то сделать с тем, что нам пришло после poll
		this->acceptProcess();
	}
}

Server::~Server() {
//	std::vector<Channel *>::iterator it = _channels.begin();
//	std::vector<Channel *>::iterator ite = _channels.end();
	_channels.clear();
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

void Server::acceptProcess() {
	pollfd nowPollfd;

	for (unsigned int i = 0; i < this->_fds.size(); i++) {
		nowPollfd = this->_fds[i];

		if ((nowPollfd.revents & POLLIN) == POLLIN) { ///можно считать данные

			if (nowPollfd.fd == this->_socketFd) { ///accept
				int clientSocket;
				sockaddr_in clientAddr;
				memset(&clientAddr, 0, sizeof(clientAddr));
				socklen_t socketLen = sizeof(clientAddr);
				clientSocket = accept(this->_socketFd, (sockaddr *) &clientAddr, &socketLen);
				if (clientSocket == -1) {
					continue;
				}
				pollfd clientPollfd = {clientSocket, POLLIN, 0};
				this->_fds.push_back(clientPollfd);
				if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1) {
					throw std::runtime_error("fcntl error");
				}
				std::cout << "new fd:" << clientSocket << std::endl;
//				std::cout << "sfd:" << this->_socketFd << std::endl;
				Client *user = new Client(clientSocket);
				this->_users.push_back(user);
			} else { ///нужно принять данные не с основного сокета, который мы слушаем(клиентского?)
				try {
//					std::cout << "fd: " << nowPollfd.fd << std::endl;
					Client *curUser = findClientbyFd(nowPollfd.fd);
					std::string receivedMsg = recvMessage(curUser->getSockFd());
					while (receivedMsg.find("\n") == std::string::npos) {
						receivedMsg.append(recvMessage(curUser->getSockFd()));
					}
					this->parser(curUser, receivedMsg);
				} catch (std::runtime_error &e) {
					std::cout << e.what() << std::endl;
				}
			}
		}
		if ((nowPollfd.revents & POLLHUP) == POLLHUP) { ///кто-то оборвал соединение
			Client *user = findClientbyFd(nowPollfd.fd);
			if (user == NULL)
				continue;
			forceQuit(*user, static_cast<std::string>("Connection lost"));
		}
	}
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

/**
 * функция для получения сообщения
 * @param fd фдшник, с которого мы получаем сообщение
 */
std::string Server::recvMessage(int fd) {
	char message[512];
	std::string res;
	ssize_t recvByte;
	memset(message, '\0', sizeof(message));
	while ((recvByte = recv(fd, message, sizeof(message), 0)) > 0) {
		message[recvByte] = 0;
		res += message;
	}
	if (recvByte <= 0) {
		throw std::runtime_error("fd " + std::to_string(fd) + " disconnected");
	}
	std::cout << "from fd " << fd << ": " << res;
	return (res);
}
//
//string Socket::tryToRecv( void ) {
//	const int _SOCK_BUFFER_SIZE = 1024;
//
//	char buf[_SOCK_BUFFER_SIZE] = {0};
//
//	string _res;
//	int rd = 0;
//	while ((rd = recv(_fd, buf, _SOCK_BUFFER_SIZE - 1, 0)) > 0) {
//		buf[rd] = 0;
//		_res += buf;
//	}
//
//	return _res;
//}