#include "../incl/malloc.h"

int    search_the_zone(t_block *block, void *ptr)
{
    int success;

    if (block->type == TINY)
    {
        success = munmap(ptr, sizeof(t_block));
        if (success == -1)
            return(-1);
        return (0);
    }
    success = munmap(ptr, sizeof(t_block));
    if (success == -1)
        return (-1);
    return (0);
}

void    ft_free(void *ptr)
{
    t_block *data_block;
    t_block *next_block;
    int     sucess;

    if (!ptr)
        return;
    
    // 1. Encontrar el bloque de memoria
    data_block = (t_block *)((char *)ptr - sizeof(t_block));
    data_block->is_free = true;
    
    // 3. Fusión de bloques adyacentes
    if (data_block->next != NULL && data_block->next->is_free == true)
    {
        next_block = data_block->next;
        data_block->size += next_block->size + sizeof(t_block);
        data_block->next = next_block->next;
    }

    if (data_block->next == NULL)
    {
        if (data_block->type == LARGE)
        {
            sucess = munmap(ptr, sizeof(t_block));
            if (sucess == -1)
            {
                print_str("Error: No se puede liberar");
                return ;
            }
            return;
        }
        if (search_the_zone(data_block, ptr) == -1)
        {   
            print_str("Error: No se puede liberar");
            return ;
        }        
        return;
    }
}