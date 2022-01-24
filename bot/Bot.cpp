

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include "Bot.hpp"


Bot::Bot(int sockfd, std::string host, std::string port, std::string pass) : fd(sockfd), _ip(host), _port(port), _password(pass), _isFirst(true) {}
Bot::~Bot(){}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void Bot::startBot() {
	int numbytes;
	char buf[512];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(this->_ip.c_str(), this->_port.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != nullptr; p = p->ai_next) {
		if ((fd = socket(p->ai_family, p->ai_socktype,
						 p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(fd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == nullptr) {
		fprintf(stderr, "client: failed to connect\n");
		exit(1);
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			  s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
	this->doRegister();
	while(true) {
		if ((numbytes = recv(fd, buf, 512 - 1, 0)) == -1) {
			perror("recv");
			exit(1);
		} else {
			buf[numbytes] = '\0';
			printf("client: received %s\n",buf);
			this->commandProcess(buf);
		}


	}


	close(fd);

}

void Bot::doRegister() {

	sendMessage("PASS " + _password + "\n" + "NICK MagicBot\nUSER 1 1 1 1\nJOIN #bot\n");
	
//	std::string passMessage = "PASS " + this->pass + "\r\n";
//	send(sockfd, passMessage.c_str(), 8, 0);
//	sleep(1);
//	send(sockfd, "NICK MagicBot\r\n", 15, 0);
//	sleep(1);
//	send(sockfd,"USER 1 1 1 1\r\n", 14,0);
}


void Bot::commandProcess(std::string message) {
	std::vector<std::string> sender = split(message, "!:");
	
	if (message.find("PRIVMSG MagicBot") != message.npos){
		if(_isFirst){
			sendMessage("PRIVMSG " + sender[0] + " " + "Задай мне вопрос с ответом да или нет и я дам тебе ответ на него!\n");
			sleep(2);
			_isFirst = false;
		}

			
		
		sendMessage("PRIVMSG " + sender[0] + " " + "Хмм...\n");
		sleep(2);
		sendMessage("PRIVMSG " + sender[0] + " " + "Дай-ка подумать...\n");
		sleep(2);
		sendMessage("PRIVMSG " + sender[0] + " " + returnAnswer() + "\n");

		sleep(2);
		sendMessage("PRIVMSG " + sender[0] + " " + "Задай мне вопрос и я дам тебе ответ на него!\n");
	}


//	send(sockfd, ("!!!" + arg + "\n").c_str(), arg.length(), SO_NOSIGPIPE);
}

std::string Bot::returnAnswer(){
	std::srand(std::time(NULL));
	int num = std::rand()%(20) + 1;
	switch (num)
	{
		case 1: return "Бесспорно";
		case 2: return "Предрешено";
		case 3: return "Никаких сомнений";
		case 4: return "Определённо да";
		case 5: return "Можешь быть уверен в этом";
		case 6: return "Мне кажется — «да»";
		case 7: return "Вероятнее всего";
		case 8: return "Хорошие перспективы";
		case 9: return "Знаки говорят — «да»";
		case 10: return "Да";
		case 11: return "Пока не ясно, попробуй снова";
		case 12: return "Спроси позже";
		case 13: return "Лучше не рассказывать";
		case 14: return "Сейчас нельзя предсказать";
		case 15: return "Сконцентрируйся и спроси опять";
		case 16: return "Даже не думай";
		case 17: return "Мой ответ — «нет»";
		case 18: return "По моим данным — «нет»";
		case 19: return "Перспективы не очень хорошие";
		case 20: return "Весьма сомнительно";

	}

}

void Bot::sendMessage(std::string msg) {
	std::cout << "To fd " << fd << ": \"" << msg;
	send(fd, msg.c_str(), msg.length(), SO_NOSIGPIPE);
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