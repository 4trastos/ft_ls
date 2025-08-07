#include "../incl/malloc.h"

void    separate_blocks(t_block *list)
{
    t_block *aux;
    t_block *next_block;

    aux = list;
    for (size_t i = 0; i < (BLOCKS_PER_ZONE -1); i++)
    {
        aux->size = TINY_MAX_SIZE;
        aux->is_free = true;

        next_block = (t_block*)((char *)aux + TINY_MAX_SIZE + sizeof(t_block));
        aux->next = next_block;
        aux = next_block;
    }
    aux->size = TINY_MAX_SIZE;
    aux->is_free = true;
    aux->next = NULL;
}

t_block *find_freeblocks_tiny_zones(t_zone *head)
{
    t_zone  *current_zone;
    t_block *found_block;

    current_zone = head;
    while (current_zone != NULL)
    {
        found_block = find_free_block(current_zone->head);
        if (found_block != NULL)
            return (found_block);
        current_zone = current_zone->next;
    }
    return (NULL);
}

void   *find_free_block(t_block *block)
{
    t_block *aux;

    aux = block;
    while (aux != NULL)
    {
        if (aux->is_free == true)
            return(aux);
        aux = aux->next;
    }
    return (NULL);
}