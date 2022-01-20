#include "Server.hpp"

Channel::Channel(const std::string& channel_name, Client &user) : _name(channel_name) {
	_key = "";
	_topic = "";
	_numUsers = 1;
	_maxUsers = 30;
	_users.push_back(&user);
	_operators.push_back(&user);
	_inviteOnly_flag = false;
	_key_flag = false;
};

Channel::Channel(const std::string& channel_name, const std::string& key, Client &user) :
		_name(channel_name), _key(key) {
	_topic = "";
	_numUsers = 1;
	_maxUsers = 30;
	_users.push_back(&user);
	_operators.push_back(&user);
	_inviteOnly_flag = false;
	_key_flag = true;
}

std::string Channel::getChannelName() const { return _name; }
std::string Channel::getTopic() const { return _topic; };
std::string Channel::getKey() const { return _key; };
int 		Channel::getNumUsers() const { return _numUsers; }
int			Channel::getMaxUsers() const { return _maxUsers; }
bool 		Channel::getKeyStatus() const { return _key_flag; }


void Channel::addUser(Client &user) {
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

void Channel::addOperator(Client &user) { _operators.push_back(&user); }

std::string Channel::sendUserList() {
	std::vector<Client *>::iterator it = _users.begin();
	std::vector<Client *>::iterator ite = _users.end();
	std::string userList;

	for (; it != ite; it++) {
		if (isOperator(*it))
			userList += "@";
		userList += (*it)->getNick();
		if (it + 1 != ite)
			userList += ", ";
	}
	return userList;
}

bool Channel::isOperator(Client *client) {
	std::vector<Client *>::iterator it = _operators.begin();
	std::vector<Client *>::iterator ite = _operators.end();

	for (; it != ite; it++) {
		if (*it == client)
			return true;
	}
	return false;
}

void Channel::setTopic(const std::string &topic) { _topic = topic; }


void Channel::sendMsgToChan (const std::string &message){

	std::vector<Client*>::iterator it= _users.begin();
	std::vector<Client*>::iterator ite= _users.end();
	for(; it !=ite; it++){
		sendMessage(message, (*it)->getSockFd());
	}
}
