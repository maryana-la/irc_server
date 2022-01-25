#include "Server.hpp"
#include <vector>
#include <arpa/inet.h>
void error(){
	std::cout << "Argument error, usage: ircserver [host:port_network:password_network] <port> <password>\n";
	exit(1);
}


#include <iostream>     ///< cout
#include <cstring>      ///< memset
#include <errno.h>      ///< errno
#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close

std::string getIP()
{
	const char* google_dns_server = "8.8.8.8";
	int dns_port = 53;

	struct sockaddr_in serv;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	//Socket could not be created
	if(sock < 0)
	{
		std::cout << "Socket error" << std::endl;
	}

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(google_dns_server);
	serv.sin_port = htons(dns_port);

	int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
	if (err < 0)
	{
		std::cout << "Error number: " << errno
				  << ". Error message: " << strerror(errno) << std::endl;
	}

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (struct sockaddr*)&name, &namelen);

	char buffer[80];
	const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
	if(p != NULL)
	{
		std::cout << "Local IP address is: " << buffer << std::endl;
	}
	else
	{
		std::cout << "Error number: " << errno
				  << ". Error message: " << strerror(errno) << std::endl;
	}

	close(sock);
	
	std::string ret = buffer;
	return (ret);
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
			Server server(arr[0], arr[1], arr[2]);
			server.init();
			server.start();
		} else {
			int port = static_cast<int>(strtol(argv[1], NULL, 10));
			if (port < 1 || port > 65535)
				error();
			Server server(getIP(), argv[1], argv[2]);
			server.init();
			server.start();
		}
	}
	catch (...) {
		std::cout << "catch all in main, check port accessibility\n";
	}
}
