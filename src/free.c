#include "../incl/malloc.h"
#include "../lib/printf/ft_printf.h"

t_zone  *find_zone_for_ptr(void *ptr)
{
    t_zone  *current;

    current = tiny_head;
    while (current)
    {
        if ((void *)current <= ptr && ptr < (void *)((char *)current + current->total_size))
            return current;
        current = current->next;
    }

    current = small_head;
    while (current)
    {
        if ((void *)current <= ptr && ptr < (void*)((char *)current + current->total_size))
            return current;
        current = current->next;
    }

    current = large_head;
    while (current)
    {
        if ((void *)current <= ptr && ptr < (void*)((char *)current + current->total_size))
            return current;
        current = current->next;
    }
    
    return (NULL);
    
}

void    remove_zone_list(t_zone *zone_to_remove)
{
    t_zone  *prev;
    t_zone  *current;

    prev = NULL;
    if (zone_to_remove->head->type == TINY)
        current = tiny_head;
    else
        current = small_head;
    while (current)
    {
        if (current == zone_to_remove)
        {
            if (prev)
                prev->next = current->next;
            else
            {
                if (zone_to_remove->head->type == TINY)
                    tiny_head = current->next;
                else
                    small_head = current->next;
            }
            return;
        }
        prev = current;
        current = current->next;
    }
}
void    remove_large_zone(t_zone *zone_to_remove)
{
    t_zone  *prev;
    t_zone  *current;

    prev = NULL;
    current = large_head;

    while (current)
    {
        if (current == zone_to_remove)
        {
            if (prev)
                prev->next = current->next;
            else
                large_head = current->next;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void    free(void *ptr)
{
    t_block *data_block;
    t_block *aux_block;
    t_block *head_block;
    t_zone  *zone;

    if (!ptr)
        return;
    
    pthread_mutex_lock(&g_malloc_mutex);
    
    // 1. Encontrar el bloque de memoria y la zona a la que pertenece.
    zone = find_zone_for_ptr(ptr);
    if (!zone)
    {
        print_str("*** Error: double free detected or invalid pointer ***");
        pthread_mutex_unlock(&g_malloc_mutex);
        exit(1);
    }

    data_block = (t_block *)((char *)ptr - BLOCK_OFFSET);
    if (data_block->is_free == true)
    {
        print_str("*** Error: double free detected ***\n");
        if (state)
            ft_printf("Dirección del puntero que ya ha sido liberado : %p\n", data_block);
        pthread_mutex_unlock(&g_malloc_mutex);
        exit(1);
    }

    if (data_block->type == LARGE)
    {
        // 2. Desenlazar la zona de la lista y liberar (LARGE).
        remove_large_zone(zone);
        if (munmap(zone, zone->total_size) == -1)
        {
            print_str("Error: munmap failed for address");
            pthread_mutex_unlock(&g_malloc_mutex);
            return;
        }
        if (state)
        {
            ft_printf("Dirección de zone       (free)    : %p\n", zone);
            ft_printf("Dirección de data_block (free)    : %p\n", data_block);
            ft_printf("Tamaño de bytes         (free)    : %u\n", (unsigned int)data_block->size);
        }
        pthread_mutex_unlock(&g_malloc_mutex);
        return;
    }
    else
    {
        data_block->is_free = true;
        if (state)
        {
            ft_printf("Dirección de zone       (free)    : %p\n", zone);
            ft_printf("Dirección de data_block (free)    : %p\n", data_block);
            ft_printf("Tamaño de bytes         (free)    : %u\n", (unsigned int)data_block->size);
        }
        
        // 3. Fusión de bloques adyacentes libres (Coalescencia)
        if (data_block->next != NULL && data_block->next->is_free == true)
        {
            aux_block = data_block->next;
            data_block->size = data_block->size + aux_block->size + BLOCK_OFFSET;
            data_block->next = aux_block->next;
            if (data_block->next != NULL)
                data_block->next->prev = data_block;
        }
        if (data_block->prev != NULL && data_block->prev->is_free == true)
        {
            aux_block = data_block->prev;
            aux_block->size = aux_block->size + data_block->size + BLOCK_OFFSET;
            aux_block->next = data_block->next;
            if (aux_block->next != NULL)
                aux_block->next->prev = aux_block;
            
            data_block = aux_block;
        }

        head_block = zone->head;
        while (head_block != NULL)
        {
            if (head_block->is_free == false)
            {
                pthread_mutex_unlock(&g_malloc_mutex);
                return;
            }
            head_block = head_block->next;
        }
        
        remove_zone_list(zone);
        if (munmap(zone, zone->total_size) == -1)
        {
            print_str("Error: munmap failed for address");
            pthread_mutex_unlock(&g_malloc_mutex);
            exit(1);
        }
        pthread_mutex_unlock(&g_malloc_mutex);
    }
}
