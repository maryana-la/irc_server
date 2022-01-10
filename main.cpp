
#include "Server.hpp"





int main(int argc, char **argv){


	
	if(argc != 3){
		std::cout << " Argument error, usage: ircserver [host:port_network:password_network] <port> <password>";
		exit(1);
	}

	Server ircserv(LOCALHOST, argv[1], argv[2]);
	ircserv.start();
	
	
	
}
