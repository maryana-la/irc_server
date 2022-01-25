#include "Server.hpp"
#include <vector>
#include <arpa/inet.h>
void error(){
	std::cout << "Argument error, usage: ircserver [host:port_network:password_network] <port> <password>\n";
	exit(1);
}
int main(int argc, char **argv) {
	if(argc != 3 && argc != 2)
		error();
	try {
		if (argc == 2) {
			std::vector<std::string> arr = split(argv[1], ": ");
			if (arr.size() != 3)
				error();

			struct sockaddr_in sa;//IP validation
			int result = inet_pton(AF_INET, arr[0].c_str(), &(sa.sin_addr));
			if (result == 0)
				error();

			int port = static_cast<int>(strtol(arr[1].c_str(), NULL, 10));
			if (port < 1 || port > 65535)
				error();

			//todo add try/catch for server init/start
			Server server(&arr[0], arr[1], arr[2]);
			server.init();
			server.start();
		} else {
			int port = static_cast<int>(strtol(argv[1], NULL, 10));
			if (port < 1 || port > 65535)
				error();
			Server server(NULL, argv[1], argv[2]);
			server.init();
			server.start();
		}
	}
	catch (...) {
		std::cout << "catch all in main, check port accessibility\n";
	}
}
