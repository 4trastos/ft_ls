#include "../incl/malloc.h"

// Función auxiliar para encontrar y desenlazar la zona de la lista LARGE
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
            if (prev != NULL)
                prev->next = current->next;
            else
                large_head = current->next;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void    ft_free(void *ptr)
{
    t_block *data_block;
    t_block *next_block;
    t_zone  *zone;
    int     sucess;

    if (!ptr)
    return;
    
    // 1. Encontrar el bloque de memoria
    data_block = (t_block *)((char *)ptr - sizeof(t_block));

    // Zona a la que perteneze
    zone = (t_zone*)((char *)data_block - sizeof(t_zone));

    if (data_block->type == LARGE)
    {
        // Si es LARGE, desenlazamos y liberamos la zona
        remove_large_zone(zone);
        sucess = munmap(zone, zone->total_size);
        if (sucess == -1)
        {
            perror("Error: No se puede liberar 01");
            return ;
        }
        printf("\n 4. MEMORIA LIBERADA \n");
        return;
    }
    
    // 2. Marcar el bloque como libre (TINY/SMALL)
    data_block->is_free = true;
    
    // 3. Fusión de bloques adyacentes
    if (data_block->next != NULL && data_block->next->is_free == true)
    {
        next_block = data_block->next;
        data_block->size += next_block->size + sizeof(t_block);
        data_block->next = next_block->next;
    }

    // 4. Comprobar si toda la zona TINY/SMALL está libre para liberarla
    // ... (Lógica que aún no has implementado) ...
  
}