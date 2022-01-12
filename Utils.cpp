#include "Server.hpp"

std:: vector<std::string> split(const std::string& line, const std::string& delimiter) {
	std::vector<std::string> args;
	char* tmp;
	tmp = std::strtok(const_cast<char *>(line.c_str()), const_cast<char *>(delimiter.c_str()));

	while (tmp != NULL) {
		args.push_back(tmp);
		tmp = strtok (NULL, " \n");
	}
	return args;
}