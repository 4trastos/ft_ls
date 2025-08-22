#include "../incl/malloc.h"
#include "../lib/printf/ft_printf.h"

void    *realloc(void *ptr, size_t size)
{
    t_zone *zone;
    t_block *block;
    t_block *aux_block;
    size_t  aligned_size;
    void    *new_ptr;
    char    *env_var;
    
    pthread_mutex_lock(&g_malloc_mutex);

    env_var = getenv("MALLOC_VERBOSE");
    if (env_var != NULL)
        state = 1;
    

    if (!ptr)
    {
        pthread_mutex_unlock(&g_malloc_mutex);
        return (malloc(size));
    }
    
    if (size == 0)
    {
        pthread_mutex_unlock(&g_malloc_mutex);
        free(ptr);
        return (NULL);
    }

    block = (t_block *)((char *)ptr - BLOCK_OFFSET);
    zone = find_zone_for_ptr(ptr);
    if (!zone)
    {
        pthread_mutex_unlock(&g_malloc_mutex);
        return (NULL);
    }

    aligned_size = (size + sizeof(void *) -1) & ~(sizeof(void *) - 1);
    if (state)
        ft_printf("DEBUG: realloc para el block: %p. Tamaño solicitado: %u. Tamaño alineado: %u\n", ptr, size, (unsigned int)aligned_size);

    if (block->type == LARGE)
    {
        pthread_mutex_unlock(&g_malloc_mutex);
        new_ptr = malloc(size);
        if (!new_ptr)
            return (NULL);
        pthread_mutex_lock(&g_malloc_mutex);
        ft_memcpy(new_ptr, ptr, (size < block->size) ? size : block->size);
        pthread_mutex_unlock(&g_malloc_mutex);
        free(ptr);
        if (state)
            ft_printf("DEBUG: Realloc en un block LARGE. Nueva dirección: %p.Nuevo tamaño: %u\n", new_ptr, (unsigned int)size);
        return (new_ptr);
    }
    else
    {
        if (aligned_size > block->size)
        {
            if (block->next != NULL && block->next->is_free == true &&
                (block->size + block->next->size + BLOCK_OFFSET) >= aligned_size) 
            {
                aux_block = block->next;
                block->size = block->size + aux_block->size + BLOCK_OFFSET;
                block->next = aux_block->next;
                if (block->next != NULL)
                    block->next->prev = block;
                if (state)
                    ft_printf("DEBUG: Realloc TINY/SMALL. Ampliando in-place. Nuevo tamaño del block: %u\n", (unsigned int)block->size);
                pthread_mutex_unlock(&g_malloc_mutex);
                return (ptr);
            }
            else
            {
                pthread_mutex_unlock(&g_malloc_mutex);
                new_ptr = malloc(size);
                if (!new_ptr)
                    return (NULL);
                pthread_mutex_lock(&g_malloc_mutex);
                ft_memcpy(new_ptr, ptr, block->size);
                pthread_mutex_unlock(&g_malloc_mutex);
                free(ptr);
                pthread_mutex_lock(&g_malloc_mutex);
                if (state)
                    ft_printf("DEBUG: Realloc TINY/SMALL. Nueva Asignación. Nueva dirección: %p. Nuevo tamaño: %u\n", new_ptr, (unsigned int)size);
                pthread_mutex_unlock(&g_malloc_mutex);
                return(new_ptr);
            }
    
        }
        else if (aligned_size <= block->size)
        {
            if (block->size >= aligned_size + BLOCK_OFFSET)
           {
               aux_block = (t_block*)((char *)block + BLOCK_OFFSET + aligned_size);
   
               aux_block->size = block->size - aligned_size - BLOCK_OFFSET;
               aux_block->is_free = true;
               aux_block->type = block->type;
               aux_block->next = block->next;
               aux_block->prev = block;
               if (aux_block->next != NULL)
                   aux_block->next->prev = aux_block;
               block->size = aligned_size;
               block->next = aux_block;
   
               if (aux_block->next != NULL &&  aux_block->next->is_free == true)
               {
                   aux_block->size = aux_block->size + aux_block->next->size + BLOCK_OFFSET;
                   aux_block->next = aux_block->next->next;
                   if (aux_block->next != NULL)
                       aux_block->next->prev = aux_block;
               }
           }
        }
    }
    if (state)
        ft_printf("DEBUG: Realloc finalizado. Dirección: %p, Tamaño final: %u\n", ptr, (unsigned int)block->size);
    pthread_mutex_unlock(&g_malloc_mutex);
    return (ptr);
}