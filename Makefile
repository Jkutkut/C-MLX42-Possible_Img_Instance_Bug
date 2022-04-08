CC=gcc
FLAGS=-Wall -Wextra -Werror
COMPILE=$(CC) $(FLAGS)

NAME=so_long
SRCS=main.c
BINS=$(SRCS:%.c=%.o)
LIB=MLX42/libmlx42.a

OS=$(shell uname -s)

ifeq ($(OS), Darwin)
	MLX_FLAGS	=	-I include -lglfw -L "/Users/${USER}/.brew/opt/glfw/lib/"
else
	MLX_FLAGS	=	-I include -ldl -lglfw
endif

all: $(NAME)

$(NAME): $(LIB) $(BINS)
	$(COMPILE) $(BINS) $(LIB) $(MLX_FLAGS) -o $(NAME)

%.o: %.c
	$(COMPILE) $(FLAGS) -c $< -o $@

$(LIB): $(dir $(LIB))
	make -C $(dir $(LIB))

$(dir $(LIB)):
	git clone https://github.com/codam-coding-college/MLX42.git


clean:
	rm -f $(BINS)

fclean: clean
	rm -f $(NAME)