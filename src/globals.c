#include "../incl/malloc.h"
#include "../lib/printf/ft_printf.h"

t_zone *tiny_head = NULL;
t_zone *small_head = NULL;
t_zone *large_head = NULL;

pthread_mutex_t g_malloc_mutex;
int g_mutex_initialized = 0;