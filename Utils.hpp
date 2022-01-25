#ifndef FT_IRC_UTILS_HPP
#define FT_IRC_UTILS_HPP

# include <iostream>

void sendMessage(const std::string &msg, int socket_fd);
int checkValidChannelName(const std::string &name);
std:: vector<std::string> split(const std::string& line, const std::string& delimiter);
std:: vector<std::string> split_args(const std::string& line);
std::string	intToString(long int num);
void sendmotd(Client &client);
void errorMain();

#endif //FT_IRC_UTILS_HPP
