#include "Server.hpp"

int main(int argc, char **argv) {
	if(argc != 3)
		errorMain();
	try {
		int port = static_cast<int>(strtol(argv[1], NULL, 10));
		if (port < 1 || port > 65535)
			errorMain();
		std::cout << "Server started\n";
		Server server(NULL, argv[1], argv[2]);
		server.begin();
		}
	catch (...) {
		std::cout << "Error, check port accessibility\n";
	}
}
