#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <sstream>
# include <iomanip>
# include <string>
# include <stdexcept>
# include <vector>

# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <unistd.h>
# include <poll.h>

# define LOCALHOST "127.0.0.1"
#define MAX_CONNECTION	1024

using namespace std;

class Commander;
class Channel;
class Server;

class Client {

private:
	int						_sockFd;
	int						_port;
	std::string				_id;
	std::string				_host;
	std::string				_nickname;
	std::string				_realname;
	std::string				_message;
	std::string				_awayMessage;
	bool					_enterPassword;
	bool					_registered;
	bool					_isOperator;

public:
	Client(int sockFd, int port, Server *serv, char *host = nullptr);
	~Client(void) {};

	std::string getNick(void) {return _nickname;}
	int			getSockFd(void) {return _sockFd;}
	std::string	getHost(void) {return _host;}
	int			getPort(void) {return _port;}
	std::string getRealname(void) {return _realname;}
	bool		getEnterPassword(void) {return _enterPassword;}
	
	std::string getMessage() { return _message; }

	void		clearMessage(void);
	void		appendMessage(std::string message);
	void		sendMessageToClient(std::string message);
//	void		registered();
};


class Server{

private:
		const std::string		_host;
		const std::string		_port;
		const std::string		_pass;
		int						_sock;
		std::vector<pollfd>		_pollfds;
		std::vector<Client *>	_clients;
		std::vector<Channel *>	_channels;
		int						_id[3];
		Commander				*_Commander;

public:
		Server(const std::string host, const std::string port, const std::string pass);
		~Server(void) {};

		int	getId(int i);
		Client *getIdClient(std::string id);

		void					createSocket(void);
		void					start(void);
		int						createClient(void);
		void					recvMessage(Client *client);

	void Fatal(std::string str);
};

#endif