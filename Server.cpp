#include "Server.hpp"


#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <array>
#include <arpa/inet.h>
#include "Server.hpp"
//#include "Client.hpp"


Server::Server(const std::string *host, const std::string &port, const std::string &password)
		: _socketFd(-1), _host(host), _port(port), _password(password) {}

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

	if (getaddrinfo(this->_host ? this->_host->c_str() : nullptr, this->_port.c_str(), &hints, &serverInfo) != 0) {
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

}

Client *Server::findUserByFd(int fd) {
	for (unsigned int i = 0; i < this->_users.size(); i++) {
		if (fd == this->_users[i]->getSockFd()) {
			return this->_users[i];
		}
	}
	return NULL;
}

void Server::acceptProcess() {
	pollfd nowPollfd;

	for (unsigned int i = 0; i < this->_fds.size(); i++) {
		nowPollfd = this->_fds[i];

		if ((nowPollfd.revents & POLLIN) == POLLIN) { ///модно считать данные

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
				std::cout << "cs:" << clientSocket << std::endl;
				std::cout << "sfd:" << this->_socketFd << std::endl;
				Client *user = new Client(clientSocket);
				this->_users.push_back(user);
			} else { ///нужно принять данные не с основного сокета, который мы слушаем(клиентского?)
				try {
//					std::cout << "fd: " << nowPollfd.fd << std::endl;
					Client *curUser = findUserByFd(nowPollfd.fd);
					this->parser(curUser, recvMessage(curUser->getSockFd()));
				} catch (std::runtime_error &e) {
					std::cout << e.what() << std::endl;
				}
			}
		}
		if ((nowPollfd.revents & POLLHUP) == POLLHUP) { ///кто-то оборвал соединение
			Client *user = findUserByFd(nowPollfd.fd);
			if (user == nullptr) {
				continue;
			}
			this->removeUser(user);
			close(_fds[i].fd);
			this->_fds.erase(_fds.begin() + i);
		}
	}
}

void Server::removeUser(Client *user) {
	for (unsigned int i = 0; i != this->_users.size(); i++) {
		if (user == _users[i]) {
//			users[i]->leaveAllChannels();
			_users.erase(_users.begin() + i);
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
	ssize_t recvByte;
	memset(message, '\0', sizeof(message));
	recvByte = recv(fd, message, sizeof(message), 0);
	if (recvByte <= 0) {
		throw std::runtime_error("recv < 0");
	}
	std::cout << "from fd" << fd << ": " << message << "" << std::endl;
	return (message);
}

