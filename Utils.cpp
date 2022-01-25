#include "Server.hpp"
#include "Utils.hpp"


void sendMessage(const std::string &msg, int socket_fd) {
	std::cout << "To fd " << socket_fd << ": \"" << msg;
	send(socket_fd, msg.c_str(), msg.length(), IRC_NOSIGNAL);
}


std:: vector<std::string> split(const std::string& line, const std::string& delimiter) {
	std::vector<std::string> args;
	char* tmp;
	tmp = std::strtok(const_cast<char *>(line.c_str()), const_cast<char *>(delimiter.c_str()));

	while (tmp != NULL) {
		args.push_back(tmp);
		tmp = strtok (NULL, const_cast<char *>(delimiter.c_str()));
	}
	return args;
}


std:: vector<std::string> split_args(const std::string& line) {
	if (line.length() > 510)
		throw "Line is too long\n";

	std::vector<std::string> args;
	size_t i = 0;
	while (i < line.length()) {
		if (line[i] == ' ')
			i++;
		else {
			std::string tmp;
			size_t begin = i;
			if (line[i] == ':') {
				i = line.length();
				begin++;
			}
			else
				i = line.find(' ', i);
			tmp.assign(line, begin, i - begin);
			args.push_back(tmp);
		}
	}
	return args;
}

std::string	intToString(long int num) {
	std::stringstream ss;
	ss << num;
	return(ss.str());
}

int checkValidChannelName(const std::string &name) {
	if (name.length() > 50 || name.find_first_of("&#+!") != 0)
		return 0;
	for (unsigned int i = 0; i < name.length(); i++) {
		if (isspace(name[i]) || iscntrl(name[i]) || name[i] == ',')
			return 0;
	}
	return 1;
}

void errorMain() {
	std::cout << "Argument error, usage: ircserver <port> <password>\n";
	exit(1);
}
