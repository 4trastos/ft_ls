NAME = libft_malloc_$HOSTTYPE.so
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g

RM = rm -f
LIB = ar rcs

SRC = src/malloc.c src/free.c src/realloc.c src/memory_zone.c lib/aux.c \
	test/test_malloc.c

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
