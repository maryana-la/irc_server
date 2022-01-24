
#ifndef FT_IRC_BOT_HPP
#define FT_IRC_BOT_HPP

#include <iostream>
#include <vector>




class Bot{
private:
	int fd;
	std::string _ip;
	std::string _port;
	std::string _password;
	bool _isFirst;
public:
	Bot(int sockfd, std::string host, std::string port, std::string pass);
	~Bot();
	void startBot();
	void doRegister();
	void doIt(std::string message);
	std::string returnAnswer();
	void sendMessage(std::string msg) ;
	std:: vector<std::string> split(const std::string& line, const std::string& delimiter);
};


#endif //FT_IRC_BOT_HPP
