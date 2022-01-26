NAME = ircServ

CC = c++

CPPFLAGS = -Wall -Wextra -Werror  -std=c++98

SRC :=\
Channel.cpp         Server.cpp          channelCMD.cpp      modeCMD.cpp         serverCommands.cpp\
Client.cpp          ServerUtils.cpp     extraCMD.cpp        privmsgCMD.cpp\
Command.cpp         Utils.cpp           main.cpp            registrationCMD.cpp\


OBJ := ${SRC:.cpp=.o}

HEADERS =Error_Reply.hpp Server.hpp

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
