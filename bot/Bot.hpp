
#ifndef FT_IRC_BOT_HPP
#define FT_IRC_BOT_HPP

#include <iostream>
#include <vector>
# include <arpa/inet.h>

class Bot{
private:
	int _fd;
	std::string _ip;
	std::string _port;
	std::string _password;
	bool _isFirst;
public:
	Bot(std::string host, std::string port, std::string pass);
	~Bot();
	void exec();
	void doIt(std::string message);
	std::string returnAnswer();
	void sendMessage(std::string msg) ;
	std:: vector<std::string> split(const std::string& line, const std::string& delimiter);
};


#endif //FT_IRC_BOT_HPP
