NAME = libft_malloc_$(HOSTTYPE).so
CC = gcc
CFLAGS = -Wall -Werror -Wextra -fPIC -g
RM = rm -f

ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

SRC = src/malloc.c src/free.c src/realloc.c src/memory_zone.c lib/aux.c \
      src/globals.c src/blocks.c lib/auxTwo.c

OBJS = $(SRC:.c=.o)

TEST_EXEC = test_malloc_app
TEST_SRC = test/test_malloc.c

all: libft_malloc.so $(TEST_EXEC)

libft_malloc.so: $(NAME)
	@ln -sf $(NAME) libft_malloc.so

$(NAME): $(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS)

$(TEST_EXEC): $(TEST_SRC) libft_malloc.so
	$(CC) $(CFLAGS) $< -L. -lft_malloc -Wl,-rpath,'$$ORIGIN' -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TEST_EXEC)

fclean: clean
	$(RM) $(NAME) libft_malloc.so

re: fclean all

.PHONY: all clean fclean re
