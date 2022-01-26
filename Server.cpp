#include "Server.hpp"

Server::Server(const std::string *host, const std::string &port, const std::string &password)
		: _socketFd(-1), _host(host), _port(port), _password(password), _maxNumberOfChannels(30) {
	_allowedIP = inet_addr("0.0.0.0");
	_operator_login.insert(std::make_pair("jjacquel", "ircserv"));
	_operator_login.insert(std::make_pair("rchelsea", "ircserv"));
}

void Server::begin() {
	//получаем сокет
	if ((_socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Connection error: socket");

	//настраиваем сокет на повторное использование адреса при перезапуске сервера
	const int trueFlag = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
		throw std::runtime_error("Connection error: setsockopt");

	//	структура позволяет обращаться элементам адреса сокета
	_sockaddr.sin_family = AF_INET; // cемейство адресов, AF_INET
	_sockaddr.sin_addr.s_addr = INADDR_ANY; // = INADDR_ANY; // используйте мой IPv4 адрес
	_sockaddr.sin_port = htons(strtol(_port.c_str(), NULL, 10)); // преобразовывает числа в порядок байтов сети

	//связываем сокет с конкретным адресом в списке res, а не со всем списком
	if (bind(_socketFd, (struct sockaddr*)&_sockaddr, sizeof(sockaddr)) < 0) {
		close(_socketFd);
		throw std::runtime_error("Connection error: bind");
	}

	//устанавливаем режим прослушивания входящих соединений на сокете
	if (listen(_socketFd, 32) < 0) {
		throw std::runtime_error("listen error");
	}
	//устанавливаем для сокета неблокирующий доступ
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl error");

	//кладем в масссив дескрипторов первый фд основного сокета
	pollfd pfd = {_socketFd, POLLIN, 0};
	_fds.push_back(pfd);

	//постоянно опрашиваем основной сокет
	std::vector<pollfd>::iterator it;
	while (true) {
		it = _fds.begin();
		if (poll(&(*it), _fds.size(), -1) == -1)
			throw std::runtime_error("poll error");
		exec();
	}

}

void Server::exec() {
	pollfd nowPollfd;
	for (unsigned int i = 0; i < _fds.size(); i++) {
		nowPollfd = _fds[i];
		if ((nowPollfd.revents & POLLIN) == POLLIN) {
			//событие пришло с фдшника основного сокета, значит есть подключение нового клиента
			//выделяем ему свой фдшник и добавляем в массив для опроса, создаём экземпляр клиента
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
					throw std::runtime_error("fcntl error");
				}
				std::cout << "new fd:" << clientSocket << std::endl;
				Client *user = new Client(clientSocket, clientAddr);
				_users.push_back(user);
			} else {
				//событие на фд клиента, значит пришло сообщение, читаем и парсим
				try {
					Client *curUser = findClientbyFd(nowPollfd.fd);
					std::string receivedMsg = recvMessage(curUser->getSockFd());
					curUser->messageAppend(receivedMsg);
					if (curUser->getReadCompleteStatus())
						parser(curUser, curUser->getMessage());
				} catch (std::runtime_error &e) {
					std::cout << e.what() << std::endl;
				}
			}
		}
		//пришло событие о дисконнекте, делетим клиента
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
