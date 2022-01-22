#include "Server.hpp"

int main(int argc, char **argv) {
	if(argc != 3){
		std::cout << " Argument error, usage: ircserver [host:port_network:password_network] <port> <password>";
		exit(1);
	}
	//todo add try/catch for server init/start
	Server server(new std::string("10.21.34.84"), argv[1], argv[2]);
	server.init();
	server.start();
}
