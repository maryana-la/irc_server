NAME = ircServ

CC = c++

CPPFLAGS = -Wall -Wextra -Werror  -std=c++98

SRC :=\
Channel.cpp         main.cpp\
Client.cpp          modeCommand.cpp\
Command.cpp         privmsgCommand.cpp\
Server.cpp          registrationCMD.cpp\
Utils.cpp           serverCommands.cpp\
joinCommand.cpp	


OBJ := ${SRC:.cpp=.o}

HEADERS =Error_Reply.hpp Server.hpp




%.o : %.cpp $(HEADERS)
	@$(CC) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@echo DONE!

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY:	all clean fclean re
