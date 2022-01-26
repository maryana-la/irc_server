

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include "Bot.hpp"


Bot::Bot(std::string host, std::string port, std::string pass) : _fd(-1), _ip(host), _port(port), _password(pass), _isFirst(true) {}

Bot::~Bot(){}


void Bot::exec() {
	int sizeReaded;
	char buffer[1024];
	struct addrinfo hints, *res;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((getaddrinfo(_ip.c_str(), _port.c_str(), &hints, &res)) != 0) {
		std::cout << "Adress error\n";
		exit(1);
	}
	
	if ((_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		std::cout << "Socket error\n";
		exit(1);
	}

	if (connect(_fd, res->ai_addr, res->ai_addrlen) == -1) {
		close(_fd);
		std::cout << "Connect error\n";
		exit(1);
	}
	
	freeaddrinfo(res);
	sendMessage("PASS " + _password + "\n" + "NICK MagicBot\nUSER 1 1 1 1\nJOIN #bot\n");
	while(1) {
		if ((sizeReaded = recv(_fd, buffer, 512 - 1, 0)) == 0) {
			std::cout << "Disconnected\n";
			close(_fd);
			exit(1);
		} else {
			buffer[sizeReaded] = '\0';
			std::cout << "Received: "<< buffer << "\n";
			doIt(buffer);
		}
	}
}

void Bot::doIt(std::string message) {
	std::vector<std::string> sender = split(message, "!:");
	
	if (message.find("PRIVMSG MagicBot") != message.npos){
		if(!_isFirst){
			sendMessage("PRIVMSG " + sender[0] + " " + ":Задай мне вопрос с ответом да или нет и я дам тебе ответ на него!\n");
			sleep(2);
			_isFirst = false;
		}
		sendMessage("PRIVMSG " + sender[0] + " " + ":Хмм...\n");
		sleep(2);
		sendMessage("PRIVMSG " + sender[0] + " " + ":Дай-ка подумать...\n");
		sleep(2);
		sendMessage("PRIVMSG " + sender[0] + " " + returnAnswer() + "\n");
		sleep(2);
		sendMessage("PRIVMSG " + sender[0] + " " + ":Задай мне вопрос и я дам тебе ответ на него!\n");
	}
}

std::string Bot::returnAnswer(){
	std::srand(std::time(NULL));
	int num = std::rand()%(20) + 1;
	switch (num)
	{
		case 1: return ":Бесспорно";
		case 2: return ":Предрешено";
		case 3: return ":Никаких сомнений";
		case 4: return ":Определённо да";
		case 5: return ":Можешь быть уверен в этом";
		case 6: return ":Мне кажется — «да»";
		case 7: return ":Вероятнее всего";
		case 8: return ":Хорошие перспективы";
		case 9: return ":Знаки говорят — «да»";
		case 10: return ":Да";
		case 11: return ":Пока не ясно, попробуй снова";
		case 12: return ":Спроси позже";
		case 13: return ":Лучше не рассказывать";
		case 14: return ":Сейчас нельзя предсказать";
		case 15: return ":Сконцентрируйся и спроси опять";
		case 16: return ":Даже не думай";
		case 17: return ":Мой ответ — «нет»";
		case 18: return ":По моим данным — «нет»";
		case 19: return ":Перспективы не очень хорошие";
		case 20: return ":Весьма сомнительно";
		default: return NULL;
	}
	return NULL;
}

void Bot::sendMessage(std::string msg) {
	std::cout << "To fd " << _fd << ": \"" << msg;
	send(_fd, msg.c_str(), msg.length(), SO_NOSIGPIPE);
}

std:: vector<std::string> Bot::split(const std::string& line, const std::string& delimiter) {
	std::vector<std::string> args;
	char* tmp;
	tmp = std::strtok(const_cast<char *>(line.c_str()), const_cast<char *>(delimiter.c_str()));

	while (tmp != NULL) {
		args.push_back(tmp);
		tmp = strtok (NULL, const_cast<char *>(delimiter.c_str()));
	}
	return args;
}
