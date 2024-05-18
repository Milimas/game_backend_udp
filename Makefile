CXX = c++

NAME = game_backend.app

CPPFLAGS = #-Wall -Wextra -Werror -std=c++98 #-g3 -fsanitize=address

SRC =	main.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
