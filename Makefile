NAME := Scop

SRC := main.cpp \
       $(wildcard src/*.cpp)

OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)

CC := c++
UNAME_S := $(shell uname -s)

CFLAGS := -Wall -Wextra -Werror -std=c++20 -Iincludes -g -fsanitize=address

ifeq ($(UNAME_S),Linux)
    LIBS := -lglfw -lGL -ldl -lm -lpthread
endif

ifeq ($(UNAME_S),Darwin)
    GLFW_PREFIX := $(shell brew --prefix glfw)
    CFLAGS += -I$(GLFW_PREFIX)/include
    LIBS := -L$(GLFW_PREFIX)/lib -lglfw \
            -framework OpenGL \
            -framework Cocoa \
            -framework IOKit \
            -framework CoreVideo \
			-g -fsanitize=address
endif

all: delete_appledouble $(NAME)

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

delete_appledouble:
	@echo "Deleting AppleDouble files..."
	@find . -name '._*' -type f -delete

.PHONY: all clean fclean re delete_appledouble
