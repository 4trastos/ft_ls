NAME = libft_malloc_$HOSTTYPE.so
CC = gcc
CFLAGS = -Wall -Werror -Wextra -fPIC -g

RM = rm -f

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

SRC = src/malloc.c src/free.c src/realloc.c src/memory_zone.c lib/aux.c \
	lib/struct.c

OBJS = $(SRC:.c=.o)

all: libft_malloc.so

libft_malloc.so: $(NAME)
	@ln -sf $(NAME) libft_malloc.so

# Crear el .so con los .o ya compilados
$(NAME): $(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS)

# Regla general para compilar .o desde .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME) libft_malloc.so

re: fclean all

.PHONY: all clean fclean re
