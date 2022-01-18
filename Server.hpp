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

# include "Error_Reply.hpp"

# define LOCALHOST "127.0.0.1"
#define MAX_CONNECTION	1024

#define NICK_VALIDSET		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_[]{}\'|"
#define CHANNEL_VALIDSET	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_[]{}\'| "

class Command;
class Channel;
class Server;
class Client;


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


public:
	Client(int sockFd, int port, Server *serv, char *host = nullptr);
	~Client(void) {};


    int			getSockFd()			{ return _sockFd; }
    int			getPort() 			{ return _port; }
    std::string getNick() 			{ return _nickname; }
	std::string getUsername()		{ return _username; }
	std::string	getHost() 			{ return _host; }
	std::string getRealname() 		{ return _realname; }
	std::string getMessage() 		{ return _message; }
    bool 		getRegisterStatus() { return _registered; }
	bool 		getPassStatus() 	{ return _passValid; }
    bool 		checkUserStatus() 	{ return((_username[0] && _host[0] && _servername[0] && _realname[0])); }

    void setNick(const std::string &name) 			{ _nickname = name; }
	void setUserName (const std::string &name) 		{ _username = name; }
	void setHostName (const std::string &name) 		{ _host = name; }
	void setServerName (const std::string &name) 	{ _servername = name; }
	void setRealName (const std::string &name) 		{ _realname = name; }
	void setPassStatus()							{ _passValid = true; }
	void setRegisterStatus()						{ _registered = true; }

	void		clearMessage();
	void		appendMessage(std::string message);
};

class Channel {
private:
	const std::string 		_name;
	std::string 			_key;
	std::string 			_topic;
	int						_numUsers;
	int 					_maxUsers;
	std::vector<Client *>	_users;
	std::vector<Client *>	_operators;
	std::vector<Client *>	_banned;
	bool 					_inviteOnly_flag;
	bool 					_key_flag;

	Channel() {}
public:
	Channel(const std::string& channel_name, Client &user) : _name(channel_name) {
		_key = "";
		_topic = "";
		_numUsers = 1;
		_maxUsers = 30;
		_users.push_back(&user);
		_operators.push_back(&user);
		_inviteOnly_flag = false;
		_key_flag = false;
	};

	Channel(const std::string& channel_name, const std::string& key, Client &user) :
			_name(channel_name), _key(key) {
		_topic = "";
		_numUsers = 1;
		_maxUsers = 30;
		_users.push_back(&user);
		_operators.push_back(&user);
		_inviteOnly_flag = false;
		_key_flag = true;
	}

	std::string getChannelName() const { return _name; }
	std::string getTopic() const { return _topic; };
	std::string getKey() const { return _key; };
	int 		getNumUsers() const { return _numUsers; }
	int			getMaxUsers() const { return _maxUsers; }
	bool 		getKeyStatus() const { return _key_flag; }


	void addUser(Client &user) {
		/* check is channel is not full */
		if (_numUsers == _maxUsers)
			throw ERR_CHANNELISFULL(_name);

		/* check if user is banned (nickname, username, host) */
		std::vector<Client *>::iterator it = _banned.begin();
		std::vector<Client *>::iterator ite = _banned.end();
		for (; it != ite; it++) {
			if ((*it)->getNick() == user.getNick() &&
					(*it)->getUsername() == user.getUsername() &&
					(*it)->getHost() == user.getHost())
				break;
		}
		if (it != ite)
			throw ERR_BANNEDFROMCHAN(_name);

		/* add user */
		//todo check if user already exists
		_users.push_back(&user);
		_numUsers++;
	}

	void addOperator(Client &user) { _operators.push_back(&user); }

	void setTopic (const std::string &topic) { _topic = topic; }
	void sendMsgToChan (const std::string &message);
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
	unsigned int 			_maxNumberOfChannels;
	int						_id[3];

public:
	Server(const std::string host, const std::string port, const std::string pass);
	~Server() {};

	int	getId(int i);
	Client *getIdClient(std::string id);

	void					createSocket();
	void					start();
	int						createClient();
	void					recvMessage(Client *client);

	void Fatal(std::string str);
    void parser(Client *client, std::string msg);


    /*
     * Commands
     */
    void passExec(Client &client, std::vector<std::string> &args);
	void userExec(Client &client, std::vector<std::string> &args);
	void nickExec(Client &client, std::vector<std::string> &args);
	void joinExec(Client &client, std::vector<std::string> &args);
	void listExec(Client &client, std::vector<std::string> &args);
	void topicShort(Client &client, const std::string& channelName);
	void topicExec(Client &client, std::vector<std::string> &args);
	void privmsgExec(Client &client, std::vector<std::string> &args);






	/*
	 * Server Utils
	 */
	Client *findClient(const std::string &clientNick);
	Channel *findChannel(const std::string &channelName);



};

void sendMessage(const std::string &msg, int socket_fd);
int checkValidChannelName(const std::string &name);
std:: vector<std::string> split(const std::string& line, const std::string& delimiter);
std:: vector<std::string> split_args(const std::string& line);

#endif
