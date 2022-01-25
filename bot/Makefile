NAME = bot

CC = c++

CPPFLAGS = -Wall -Wextra -Werror  -std=c++98

SRC :=Bot.cpp startBot.cpp

OBJ := ${SRC:.cpp=.o}

HEADERS =Bot.hpp

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
