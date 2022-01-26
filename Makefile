NAME = ircServ

CC = c++

CPPFLAGS = -Wall -Wextra -Werror -g -std=c++98 -pedantic

SRC :=\
main.cpp 	Channel.cpp	Utils.cpp           privmsgCMD.cpp\
Client.cpp          channelCMD.cpp      registrationCMD.cpp\
Parser.cpp         extraCMD.cpp        serverCommands.cpp\
Server.cpp 			ServerUtils.cpp     modeCMD.cpp

OBJ := ${SRC:.cpp=.o}

HEADERS =Channel.hpp Error_Reply.hpp Utils.hpp Client.hppServer.hpp

BOT	= bot/bot

%.o : %.cpp $(HEADERS)
	@$(CC) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(BOT) $(OBJ)
	@$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@echo DONE!

$(BOT):
	$(MAKE) -C bot

clean:
	rm -rf $(OBJ)
	$(MAKE) -C bot clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C bot fclean

re: fclean all

.PHONY:	all clean fclean re
