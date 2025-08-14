#include "../incl/malloc.h"

t_zone  *find_zone_for_ptr(void *ptr)
{
    t_zone  *current;

    current = tiny_head;
    while (current)
    {
        if ((void *)current <= ptr && ptr < (void *)(char *)current + current->total_size)
            return current;
        current = current->next;
    }

    current = small_head;
    while (current)
    {
        if ((void *)current <= ptr && ptr < (void*)(char *)current + current->total_size)
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
    
    // 1. Encontrar el bloque de memoria y la zona a la que pertenece.
    data_block = (t_block *)((char *)ptr - sizeof(t_block));
    zone = find_zone_for_ptr(ptr);
    if (!zone)
    return;
    printf("Dirección de zone (free): %p\n", zone);
    printf("Dirección de data_block (free) : %p\n", data_block);

    if (data_block->type == LARGE)
    {
        // 2. Desenlazar la zona de la lista y liberar (LARGE).
        remove_large_zone(zone);
        if (munmap(zone, zone->total_size) == -1)
        {
            perror("Error: No se puede liberar LARGE");
            return;
        }
        return;
    }
    else
    {
        data_block->is_free = true;
        
        // 3. Fusión de bloques adyacentes libres (Coalescencia)
        if (data_block->next != NULL && data_block->next->is_free == true)
        {
            aux_block = data_block->next;
            data_block->size = data_block->size + aux_block->size + sizeof(t_block);
            data_block->next = aux_block->next;
            if (data_block->next != NULL)
                data_block->next->prev = data_block;
        }
        if (data_block->prev != NULL && data_block->prev->is_free == true)
        {
            aux_block = data_block->prev;
            aux_block->size = aux_block->size + data_block->size + sizeof(t_block);
            aux_block->next = data_block->next;
            if (aux_block->next != NULL)
                aux_block->next->prev = aux_block;
            
            data_block = aux_block;
        }

        head_block = zone->head;
        while (head_block != NULL)
        {
            if (head_block->is_free == false)
                return;
            head_block = head_block->next;
        }
        
        remove_zone_list(zone);
        if (munmap(zone, zone->total_size) == -1)
            perror("Error: No se puede liberar la zona TINY/SMALL");
    }
}
