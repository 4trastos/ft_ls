#include "../incl/malloc.h"

void    separate_Sblocks(t_block *list)
{
    t_block *aux;
    t_block *next_block;

    aux = list;
    for (size_t i = 0; i < (BLOCKS_PER_ZONE -1); i++)
    {
        aux->size = SMALL_MAX_SIZE;
        aux->is_free = true;

        next_block = (t_block*)((char *)aux + SMALL_MAX_SIZE + sizeof(t_block));
        aux->next = next_block;
        aux = next_block;
    }
    aux->size = SMALL_MAX_SIZE;
    aux->is_free = true;
    aux->next = NULL;
}