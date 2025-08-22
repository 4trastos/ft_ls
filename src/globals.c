#include "../incl/malloc.h"
#include "../lib/printf/ft_printf.h"

t_zone *tiny_head = NULL;
t_zone *small_head = NULL;
t_zone *large_head = NULL;

// Mutex para la protección multihilo.
pthread_mutex_t g_malloc_mutex;

// Control para garantizar que la inicialización del mutex se haga una única vez.
pthread_once_t  g_mutex_once = PTHREAD_ONCE_INIT;

// Bandera para el modo "verbose" del depurador.
int state = 0;