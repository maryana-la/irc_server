#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <sstream>
# include <iomanip>
# include <string>
# include <cstring>
# include <stdexcept>
# include <vector>
# include <set>
# include <map>

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
#include <errno.h>
#include <netinet/in.h>



# include "Client.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include "Error_Reply.hpp"

#define NICK_VALIDSET		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_[]{}\'|"


class Channel;
class Server;
class Client;


class Server {
private:
	int _socketFd;
	sockaddr_in _sockaddr;
	in_addr_t _allowedIP;
	const std::string *_host;
	const std::string &_port;
	const std::string &_password;
	std::vector<pollfd> _fds;
	std::vector<Client *> _users;
	std::vector<Channel *> _channels;
	std::set<Client *> _operators;
	std::map<std::string, std::string> _operator_login;
	int _maxNumberOfChannels;

public:
	Server(const std::string *host, const std::string &port, const std::string &password);
	virtual ~Server();

	void begin();
	void exec();
	std::string recvMessage(int fd);
	Client *findClientbyFd(int fd);
	void parser(Client *client, std::string msg);

	std::string getHost() const { return *_host; }


    /*
     * Commands
     */
	/* registration commands */
    void passExec(Client &client, std::vector<std::string> &args);
	void userExec(Client &client, std::vector<std::string> &args);
	void nickExec(Client &client, std::vector<std::string> &args);

	/* channel commands */
	void joinExec(Client &client, std::vector<std::string> &args);
	void topicExec(Client &client, std::vector<std::string> &args);
	void partExec (Client &client, std::vector<std::string> &args);
	void kickExec (Client &client, std::vector<std::string> &args);

	/* privmsg command */
	void privmsgExec(Client &client, std::vector<std::string> &args);
	void noticeExec(Client &client, std::vector<std::string> &args);

	/* mode */
	void modeExec(Client &client, std::vector<std::string> &args);
	void setChannelModes(Client &client, std::vector<std::string> &args);
	void setUserModes(Client &client, std::vector<std::string> &args);

	/* server commands*/
	void operExec(Client &client, std::vector<std::string> &args);
	void quitExec(Client &client, std::vector<std::string> &args);
	void forceQuit(Client &client);
	void killExec(Client &client, std::vector<std::string> &args);
	void exitExec(Client &client);

	/* extra commands */
	void listExec(Client &client, std::vector<std::string> &args);
	void namesExec(Client &client, std::vector<std::string> &args);
	void pingExec(Client &client, std::vector<std::string> &args);
	void pongExec(Client &client, std::vector<std::string> &args);

	/*
	 * Server Utils
	 */
	Client *findClient(const std::string &clientNick);
	Channel *findChannel(const std::string &channelName);
	void sendTopic(Client &client, const std::string& channelName);
	void sendUsers(Client &client,Channel &channel);

	bool isServerOperator(Client *client);
	void removeClient(Client *client);
	void removeOperator(Client *client);
	void leaveChannel(Client &client, Channel *channel);

	void standartReply(Client &client, Channel *channel, const std::string &command, const std::string &param);
	std::string prefixCompose(Client &client);
};


#endif
