#include "../incl/malloc.h"

void    free(void *ptr)
{
    t_zone  *zone;

    if (!ptr)
        return;
    zone = ptr;
}