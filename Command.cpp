#include "Server.hpp"

void Server::parser(Client *client, std::string msg) {


	std::vector<std::string> common;
	//	msg.erase(std::remove(msg.begin(), msg.end(), ':'), msg.end());
	common = split(msg, "\r\n");

	/* find command and execute */

	for (unsigned int i = 0; i < common.size(); i++)
	{
		try
		{
			std::vector<std::string> args = split_args(common[i]);
			/* if client is not registered yet */
			if (!(args[0] == "PASS" || args[0] == "pass" || args[0] == "USER" || args[0] == "user" || args[0] == "NICK" || args[0] == "nick" || args[0] == "PART" || args[0] == "part" || args[0] == "KICK" || args[0] == "kick")) {
				if (!client->getRegisterStatus())
					throw static_cast<std::string>(ERR_NOTREGISTERED);
			}

			// todo replace with switch case
			if (args[0] == "PASS" || args[0] == "pass")
				passExec(*client, args);
			else if (args[0] == "USER" || args[0] == "user")
				userExec(*client, args);
			else if (args[0] == "NICK" || args[0] == "nick")
				nickExec(*client, args);
			else if (args[0] == "JOIN" || args[0] == "join")
				joinExec(*client, args);
			else if (args[0] == "LIST" || args[0] == "list")
				listExec(*client, args);
			else if (args[0] == "PRIVMSG" || args[0] == "privmsg")
				privmsgExec(*client, args);
			else if (args[0] == "PING" || args[0] == "ping")
				pingExec(*client, args);
			else if (args[0] == "TOPIC" || args[0] == "topic")
				topicExec(*client, args);
			else if (args[0] == "PART" || args[0] == "part")
				partExec(*client, args);
			else if (args[0] == "KICK" || args[0] == "kick")
				kickExec(*client, args);
			else if (args[0] == "MODE" || args[0] == "mode")
				modeExec(*client, args);
//			else if (args[0] == "PONG" || args[0] == "pong")
//				pongExec(*client, args);
			else if (args[0] == "NAMES" || args[0] == "names")
				namesExec(*client, args);
			else if (args[0] == "OPER" || args[0] == "oper")
				operExec(*client, args);
			else if (args[0] == "QUIT" || args[0] == "quit")
				quitExec(*client, args);
			else if (args[0] == "KILL" || args[0] == "kill")
				killExec(*client, args);
			else if (args[0] == "EXIT" || args[0] == "exit")
				exitExec(*client);



			//todo clear memory for args in the end
//			args.clear();
//			client->clearMessage();
		}
		catch (std::string &msg) {
			sendMessage(msg, client->getSockFd());
		}
		catch (std::exception &e) {
			sendMessage(e.what(), client->getSockFd());
			std::cout << e.what() << "\n"; //cout to server
		}
		catch (...) {
			std::cout << "Unknown mistake. Reload project\n";
		}
//		args.clear();
		client->clearMessage();
	}
}
