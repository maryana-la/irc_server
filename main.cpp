
#include "Server.hpp"





int main(int argc, char **argv){


	
	if(argc != 3){
		std::cout << " Argument error, usage: ircserver [host:port_network:password_network] <port> <password>";
		exit(1);
	}


	Server server(NULL, argv[1], argv[2]);
	server.init();
	server.start();

	
	
	
}
