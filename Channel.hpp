#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

# include <iostream>
# include <vector>
# include <set>
# include "Error_Reply.hpp"

# include "Client.hpp"

class Channel {
private:
	const std::string 		_name;
	std::string 			_key;
	std::string 			_topic;
	long int				_maxUsers;
	std::vector<Client *>	_users;
	std::set<Client *>		_operators;
	bool 					_keyFlag;
	bool 					_topicOperOnly;
	bool 					_maxUsersFlag;

	Channel() {}

public:
	Channel(const std::string& channel_name, Client &user);
	Channel(const std::string& channel_name, const std::string& key, Client &user);

	/*
	 *  GETTERS
	 */
	std::string getChannelName() const;
	std::string getTopic() const;
	std::string getKey() const;
	int 		getNumUsers() const;
	long int	getMaxUsers() const;
	bool 		getKeyStatus() const;
	bool 		getTopicOperatorsOnly() const;
	bool 		getMaxUsersFlag() const;


	/*
	 *  SETTERS
	 */
	void setTopic (const std::string &topic);
	void setTopicOperOnly(bool status);
	void setMaxUsers (long int num);
	void setKey(std::string &password);
	void setKeyFlag(bool status);
	void setMaxUsersFlag(bool status);


	/*
	 * OTHER FUNCTIONS
	 */
	void addUser(Client &user);
	void addOperator(Client &user);

	void deleteOperator(Client &user);
	void deleteUser(Client &client);

	std::string sendUserList(bool printInvisibleUsers);
	bool isOperator(Client *client);
	bool isChannelUser (Client *client);

	void sendMsgToChan(const std::string &message, Client *client, bool includingClient);
	void receiveMsgOfAllChannelUsers(Client &client, Channel *channel);
};


#endif //FT_IRC_CHANNEL_HPP
