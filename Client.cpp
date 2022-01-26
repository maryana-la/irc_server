#include <netinet/in.h>
#include "Client.hpp"

Client::Client(int socketFd, struct sockaddr_in address) : _sockFd(socketFd) {
	_nickname = "";
	_passValid = false;
	_registered = false;
	_isInvisible = false;
	sockaddr = address;
	char ip[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &sockaddr.sin_addr, ip, INET_ADDRSTRLEN );
	_host = ip;
}

Client::~Client() {}

int	Client::getSockFd() const { return _sockFd; }
int	Client::getPort() const { return _port; }
std::string Client::getNick() const	{ return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string	Client::getHost() const	{ return _host; }
std::string Client::getRealname() const	{ return _realname; }
std::string Client::getMessage() const { return _message; }
std::string Client::getAwayMessage() const { return _awayMessage; }
bool Client::getRegisterStatus() const { return _registered; }
bool Client::getPassStatus() const { return _passValid; }
bool Client::getInvisibleStatus() const { return _isInvisible; }
bool Client::getReadCompleteStatus() const { return _readIsComplete; }
bool Client::checkUserStatus() const { return((_username[0] && _servername[0] && _realname[0])); }

void Client::setNick(const std::string &name) 			{ _nickname = name; }
void Client::setUserName (const std::string &name) 		{ _username = name; }
void Client::setHostName (const std::string &name) 		{ _host = name; }
void Client::setServerName (const std::string &name) 	{ _servername = name; }
void Client::setRealName (const std::string &name) 		{ _realname = name; }
void Client::setAwayMessage (const std::string &msg)	{ _awayMessage = msg; }
void Client::setPassStatus()							{ _passValid = true; }
void Client::setRegisterStatus()						{ _registered = true; }
void Client::setInvisibleStatus(bool status) 			{ _isInvisible = status; }

void Client::messageAppend (std::string &msg) {
	if (_message.empty())
		_message = msg;
	else
		_message.append(msg);
	if (_message.find_last_of("\n") == _message.size() - 1)
		_readIsComplete = true;
	else
		_readIsComplete = false;
}

void Client::clearMessage () {
	_message.clear();
	_readIsComplete = false; }

bool operator== (const Client &lhs, const Client &rhs) {
	return (lhs.getNick() == rhs.getNick());
}

bool operator!= (const Client &lhs, const Client &rhs) {
	return (lhs.getNick() != rhs.getNick());
}
