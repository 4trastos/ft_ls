#include "../incl/malloc.h"

void    init_struct(t_block **ram)
{
    t_block *aux;

    aux = *ram;
    aux->is_free = false;
    aux->next = NULL;
    aux->size = 0;
}

void    init_structZ(t_zone **zone)
{
    t_zone  *aux;

    aux = *zone;
    aux->head = NULL;
    aux->next = NULL;
    aux->total_size = 0;
}