#include "../incl/malloc.h"

void    *realloc(void *ptr, size_t size)
{
    t_zone *zone;
    t_block *block;
    t_block *aux_block;
    void    *new_ptr;

    if (!ptr)
        return (malloc(size));
    
    if (size == 0)
    {
        free(ptr);
        return (NULL);
    }

    block = (t_block *)((char *)ptr - sizeof(t_block));
    zone = find_zone_for_ptr(ptr);
    if (!zone)
        return (NULL);
    printf("Dirección de zone       (realloc) : %p\n", zone);
    printf("Dirección de block      (realloc) : %p\n", block);
    printf("Tamaño de bytes         (realloc) : %ld\n", size);

    if (block->type == LARGE)
    {
        new_ptr = malloc(size);
        if (!new_ptr)
            return (NULL);
        ft_memcpy(new_ptr, ptr, (size < block->size) ? size : block->size);
        free(ptr);
        return (new_ptr);
    }
    else
    {
        if (size > block->size)
        {
            if (block->next != NULL && block->next->is_free == true &&
                (block->size + block->next->size + sizeof(t_block)) > size) 
            {
                aux_block = block->next;
                block->size = block->size + aux_block->size + sizeof(t_block);
                block->next = aux_block->next;
                if (block->next != NULL)
                    block->next->prev = block;
                return (ptr);
            }
            else
            {
                new_ptr = malloc(size);
                if (!new_ptr)
                    return (NULL);
                ft_memcpy(new_ptr, ptr, block->size);
                free(ptr);
                return(new_ptr);
            }
    
        }
        else if (size < block->size)
        {
            if (block->size - size > sizeof(t_block))
           {
               aux_block = (t_block*)((char *)block + sizeof(t_block) + size);
   
               aux_block->size = block->size - size - sizeof(t_block);
               aux_block->is_free = true;
               aux_block->type = block->type;
               aux_block->next = block->next;
               aux_block->prev = block;
   
               if (aux_block->next != NULL)
                   aux_block->next->prev = aux_block;
               
               block->size = size;
               block->next = aux_block;
   
               if (aux_block->next != NULL &&  aux_block->next->is_free == true)
               {
                   aux_block->size = aux_block->size + aux_block->next->size + sizeof(t_block);
                   aux_block->next = aux_block->next->next;
                   if (aux_block->next != NULL)
                       aux_block->next->prev = aux_block;
               }
           } 
        }
    }
    return (ptr);
}