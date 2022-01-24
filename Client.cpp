#include "Server.hpp"

//Client::Client(int sockFd, int port, Server *serv, char *host) : _sockFd(sockFd), _port(port), _host(host){
//	_nickname = "";
//	_realname = "";
//	_awayMessage = "";
//	_passValid = false;
//	_id = serv->getId(0) + serv->getId(1) + serv->getId(2);
//}

Client::~Client() {}

int	Client::getSockFd() const { return _sockFd; }
int	Client::getPort() const { return _port; }
std::string Client::getNick() const	{ return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string	Client::getHost() const	{ return _host; }
std::string Client::getRealname() const	{ return _realname; }
std::string Client::getMessage() const { return _message; }
bool Client::getRegisterStatus() const { return _registered; }
bool Client::getPassStatus() const { return _passValid; }
bool Client::getInvisibleStatus() const { return _isInvisible; }
bool Client::checkUserStatus() const { return((_username[0] && _host[0] && _servername[0] && _realname[0])); }

void Client::setNick(const std::string &name) 			{ _nickname = name; }
void Client::setUserName (const std::string &name) 		{ _username = name; }
void Client::setHostName (const std::string &name) 		{ _host = name; }
void Client::setServerName (const std::string &name) 	{ _servername = name; }
void Client::setRealName (const std::string &name) 		{ _realname = name; }
void Client::setPassStatus()							{ _passValid = true; }
void Client::setRegisterStatus()						{ _registered = true; }
void Client::setInvisibleStatus(bool status) 			{ _isInvisible = status; }

//void Client::clearMessage() { _message.clear(); }
//
//
//void Client::appendMessage(std::string message) {
//	_message.append(message);
//	_message.erase(_message.find_last_not_of("\n") + 1);
//	_message.append("\n");
//}

bool operator== (const Client &lhs, const Client &rhs) {
	return (lhs.getNick() == rhs.getNick());
}

bool operator!= (const Client &lhs, const Client &rhs) {
	return (lhs.getNick() != rhs.getNick());
}