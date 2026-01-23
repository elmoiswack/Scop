NAME := Scop

SRC := main.cpp \
       $(wildcard src/*.cpp)

OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

CC := c++
CFLAGS := -Wall -Wextra -Werror -std=c++20 -g -fsanitize=address

LIBS := -lglfw -lGL -ldl -lm -lpthread -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
