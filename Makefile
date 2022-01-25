NAME = ircServ

CC = c++

CPPFLAGS = -Wall -Wextra -Werror -g -std=c++98 -pedantic

SRC :=\
main.cpp 	Channel.cpp	Utils.cpp           privmsgCMD.cpp\
Client.cpp          channelCMD.cpp      registrationCMD.cpp\
Parser.cpp         extraCMD.cpp        serverCommands.cpp\
Server.cpp 			ServerUtils.cpp     modeCMD.cpp

OBJ := ${SRC:.cpp=.o}

HEADERS =Channel.hpp Error_Reply.hpp Utils.hpp Client.hpp Server.hpp

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
