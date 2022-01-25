#include "Bot.hpp"

int main(int argc, char **argv)
{
	if (argc != 4){
		std::cout << "Wrong params: ./bot <host> <port> <pass>\n";
		return 1;
	}
	
	struct sockaddr_in sa;//IP validation
	int result = inet_pton(AF_INET, argv[1], &(sa.sin_addr));
	if (result == 0){
		std::cout << "Wrong params: ./bot <host> <port> <pass>\n";
		return 1;
	}
		

	int port = static_cast<int>(strtol(argv[2], NULL, 10));
	if (port < 1 || port > 65535){
		std::cout << "Wrong params: ./bot <host> <port> <pass>\n";
		return 1;
	}

	Bot bot(argv[1], argv[2], argv[3]);
	bot.exec();
}
