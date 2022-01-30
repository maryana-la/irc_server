#include "Channel.hpp"
#include "Utils.hpp"

Channel::Channel(const std::string& channel_name, Client &user) : _name(channel_name) {
	_key = "";
	_topic = "";
	_maxUsers = 30;
	_users.push_back(&user);
	_operators.insert(&user);
	_keyFlag = false;
	_topicOperOnly = false;
	_maxUsersFlag =false;
}

Channel::Channel(const std::string& channel_name, const std::string& key, Client &user) :
		_name(channel_name), _key(key) {
	_topic = "";
	_maxUsers = 30;
	_users.push_back(&user);
	_operators.insert(&user);
	_keyFlag = true;
	_topicOperOnly = false;
	_maxUsersFlag =false;
}

std::string Channel::getChannelName() const { return _name; }
std::string Channel::getTopic() const { return _topic; }
std::string Channel::getKey() const { return _key; }
int 		Channel::getNumUsers() const { return static_cast<int>(_users.size()); }
long int	Channel::getMaxUsers() const { return _maxUsers; }
bool 		Channel::getKeyStatus() const { return _keyFlag; }
bool 		Channel::getTopicOperatorsOnly() const { return _topicOperOnly; }
bool 		Channel::getMaxUsersFlag() const { return _maxUsersFlag; }


void Channel::addUser(Client &user) {
	/* check is channel is not full */
	if (getMaxUsersFlag() && getNumUsers() >= _maxUsers)
		throw ERR_CHANNELISFULL(user.getNick(), _name);
	/* check if already in group */
	std::vector<Client *>::iterator itU = _users.begin();
	std::vector<Client *>::iterator iteU = _users.end();
	for (; itU != iteU; itU++) {
		if ((*itU)->getNick() == user.getNick())
			return;
	}
	/* add user */
	_users.push_back(&user);
}

void Channel::addOperator(Client &user) { _operators.insert(&user); }

void Channel::deleteOperator(Client &user) { 
	_operators.erase(&user);
	if(_operators.empty() && !_users.empty()) {
		addOperator(*_users[0]);
		std::string msg = ":" + user.getNick() + "!" + user.getUsername() + "@" + user.getHost() + " MODE " + getChannelName() + " +o " + _users[0]->getNick() + "\n";
		sendMsgToChan(msg, &user, true);
	}
}

void Channel::deleteUser(Client &client) {
	std::vector<Client *>::iterator it = _users.begin();
	std::vector<Client *>::iterator ite = _users.end();
	for (; it != ite; it++) {
		if ((*it)->getNick() == client.getNick())
			_users.erase(it);
	}
}

std::string Channel::sendUserList(bool printInvisibleUsers) {
	std::vector<Client *>::iterator it = _users.begin();
	std::vector<Client *>::iterator ite = _users.end();
	std::string userList;

	for (; it != ite; it++) {
		if (printInvisibleUsers || !(*it)->getInvisibleStatus()) {
			if (!userList.empty())
				userList += " ";
			if (isOperator(*it))
				userList += "@";
			userList += (*it)->getNick();
		}
	}
	return userList;
}

bool Channel::isOperator(Client *client) {
	std::set<Client *>::iterator it = _operators.begin();
	std::set<Client *>::iterator ite = _operators.end();

	for (; it != ite; it++) {
		if (*it == client)
			return true;
	}
	return false;
}

bool Channel::isChannelUser (Client *client) {
	std::vector<Client *>::iterator it = _users.begin();
	std::vector<Client *>::iterator ite = _users.end();

	for (; it != ite; it++) {
		if ((*it)->getNick() == client->getNick())
			return true;
	}
	return false;
}

void Channel::setTopic(const std::string &topic) { _topic = topic; }

void Channel::setTopicOperOnly(bool status) { _topicOperOnly = status; }

void Channel::setMaxUsers (long int num) {
	if (num > 0)
		_maxUsers = num;
}

void Channel::setKey(std::string &password) { _key = password; }

void Channel::setKeyFlag(bool status) { _keyFlag = status; }

void Channel::setMaxUsersFlag(bool status) { _maxUsersFlag = status; }

void Channel::sendMsgToChan(const std::string &message, Client *client, bool includingClient)
{

	std::vector<Client*>::iterator it= _users.begin();
	std::vector<Client*>::iterator ite= _users.end();
	for(; it !=ite; it++) {
		if((*it)->getNick() != client->getNick() || includingClient)
			sendMessage(message, (*it)->getSockFd());
	}
}

void Channel::receiveMsgOfAllChannelUsers(Client &client, Channel *channel) {

	std::vector<Client *>::iterator  it = _users.begin();
	std::vector<Client*>::iterator ite= _users.end();
	for (; it != ite; it++) {
		std::string msg;
		msg = ":" + (*it)->getNick() + "!" + (*it)->getUsername() + "@" + (*it)->getHost() + " " + "JOIN" + " :" +
			  channel->getChannelName() + "\n";
		sendMessage(msg, client.getSockFd());
	}
}
