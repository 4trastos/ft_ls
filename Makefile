NAME = ft_ls
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g

RM = rm -f
LIB = ar rcs

SRC = src/main.c

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(OBJS)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re



