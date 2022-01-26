#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

# include <iostream>
#include <netinet/in.h>


# include <sys/poll.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include <poll.h>
# include <cstdlib>
# include <algorithm>

#include <cstring>

class Client {
private:
	int						_sockFd;
	int						_port;
	std::string				_id;
	std::string				_nickname;
	std::string             _username;
	std::string				_host;
	std::string				_servername;
	std::string				_realname;
	std::string				_message;
	std::string				_awayMessage;
	bool    _passValid;
	bool    _registered;
	bool	_isInvisible;
	bool 	_readIsComplete;

	struct sockaddr_in          sockaddr;

public:
	Client(int socketFd, struct sockaddr_in address);
	~Client();

	int			getSockFd() const;
	int			getPort() const;
	std::string getNick() const;
	std::string getUsername() const;
	std::string	getHost() const;
	std::string getRealname() const;
	std::string getMessage() const;
	bool 		getRegisterStatus() const;
	bool 		getPassStatus() const;
	bool 		getInvisibleStatus() const;
	bool 		getReadCompleteStatus() const;
	bool 		checkUserStatus() const;

	void setNick(const std::string &name);
	void setUserName (const std::string &name);
	void setHostName (const std::string &name);
	void setServerName (const std::string &name);
	void setRealName (const std::string &name);

	void setPassStatus();
	void setRegisterStatus();
	void setInvisibleStatus(bool status);
	void messageAppend (std::string &msg);
	void clearMessage ();
};

#endif //FT_IRC_CLIENT_HPP
