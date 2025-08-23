#include "../incl/malloc.h"
#include "../lib/printf/ft_printf.h"

t_zone *tiny_head = NULL;
t_zone *small_head = NULL;
t_zone *large_head = NULL;

int state = 0;