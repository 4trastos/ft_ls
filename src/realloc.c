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

    if (g_mutex_initialized == 0)
    {
        if (pthread_mutex_init(&g_malloc_mutex, NULL) != 0)
            return (NULL);
        g_mutex_initialized = 1;
    }

    env_var = getenv("MALLOC_VERBOSE");
    if (env_var != NULL)
        state = 1;
    
    pthread_mutex_lock(&g_malloc_mutex);

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

    if (block->type == LARGE)
    {
        pthread_mutex_unlock(&g_malloc_mutex);
        new_ptr = malloc(size);
        if (!new_ptr)
            return (NULL);
        ft_memcpy(new_ptr, ptr, (size < block->size) ? size : block->size);
        free(ptr);
        if (state)
        {
            ft_printf("Dirección de zone       (realloc) : %p\n", zone);
            ft_printf("Dirección de block      (realloc) : %p\n", block);
            ft_printf("Tamaño de block         (realloc) : %u\n", (unsigned int)block->size);
            ft_printf("Nueva dirección block   (realloc) : %p\n", new_ptr);
            ft_printf("Nuevo tamaño de block   (realloc) : %u\n", (unsigned int)size);
        }
        return (new_ptr);
    }
    else
    {
        if (size > block->size)
        {
            if (block->next != NULL && block->next->is_free == true &&
                (block->size + block->next->size + BLOCK_OFFSET) >= size) 
            {
                aligned_size = (size + sizeof(void *) - 1) & ~(sizeof(void *) - 1);
                block->next->size = block->next->size - block->size;
                block->size = block->size + aligned_size;
                if (state)
                {
                    ft_printf("Dirección de zone       (realloc) : %p\n", zone);
                    ft_printf("Dirección de block      (realloc) : %p\n", block);
                    ft_printf("Tamaño de block         (realloc) : %u\n", (unsigned int)block->size);
                    ft_printf("Nuevo tamaño de block   (realloc) : %u\n", (unsigned int)aligned_size);
                }
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
                {
                    ft_printf("Dirección de zone       (realloc) : %p\n", zone);
                    ft_printf("Dirección de block      (realloc) : %p\n", block);
                    ft_printf("Tamaño de block         (realloc) : %u\n", (unsigned int)block->size);
                    ft_printf("Nueva dirección block   (realloc) : %p\n", new_ptr);
                    ft_printf("Nuevo tamaño de block   (realloc) : %u\n", (unsigned int)size);
                }
                pthread_mutex_unlock(&g_malloc_mutex);
                return(new_ptr);
            }
    
        }
        else if (size < block->size)
        {
            if (block->size - size > BLOCK_OFFSET)
           {
               aux_block = (t_block*)((char *)block + BLOCK_OFFSET + size);
   
               aux_block->size = block->size - size - BLOCK_OFFSET;
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
                   aux_block->size = aux_block->size + aux_block->next->size + BLOCK_OFFSET;
                   aux_block->next = aux_block->next->next;
                   if (aux_block->next != NULL)
                       aux_block->next->prev = aux_block;
               }
           } 
        }
    }
    if (state)
    {
        ft_printf("Dirección de zone       (realloc) : %p\n", zone);
        ft_printf("Dirección de block      (realloc) : %p\n", block);
        ft_printf("Tamaño de block         (realloc) : %u\n", (unsigned int)block->size);
        ft_printf("Nuevo tamaño de block   (realloc) : %u\n", (unsigned int)size);
    }
    pthread_mutex_unlock(&g_malloc_mutex);
    return (ptr);
}