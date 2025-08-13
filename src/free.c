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

    if (!ptr)
        return;
    
    // 1. Encontrar el bloque de memoria y la zona a la que pertenece.
    data_block = (t_block *)((char *)ptr - sizeof(t_block));
    zone = (t_zone*)((char *)data_block - sizeof(t_zone));

    if (data_block->type == LARGE)
    {
        // 2. Desenlazar la zona de la lista y la liberar (LARGE).
        remove_large_zone(zone);
        if (munmap(zone, zone->total_size) == -1)
        {
            perror("Error: No se puede liberar 01");
            return;
        }
        printf("\n 4. MEMORIA LIBERADA \n");
        return;
    }
    else
    {
        data_block->is_free = true;
        
        // 3. Fusión de bloques adyacentes (Coalescencia)
        // FUSION HACIA ADELANTE
        if (data_block->next != NULL && data_block->next->is_free == true)
        {
            next_block = data_block->next;
            data_block->size += next_block->size + sizeof(t_block);
            data_block->next = next_block->next;
        }
    
        // AÑADIR AQUI LA FUSIÓN HACIA ATRÁS
        // Para hacer esto, necesito una forma de encontrar el bloque anterior.
        // Esto puede implicar una doble lista enlazada o recorrer la lista desde el principio.
        // Si encuentro que el bloque anterior está libre lo fusiono con el actual.
    
        // 4. Comprobar si toda la zona TINY/SMALL está libre para liberarla
        // Después de fusionar hay que recorrer la lista de bloques de la zona actual.
        // Si encuentro que todos los bloques están marcados como 'is_free',
        // significa que la zona entera puede ser liberada con munmap.
        
        // AÑADIR AQUI LA LÓGICA PARA LIBERAR ZONAS COMPLETAMENTE VACÍAS
        // Esto evita la fragmentación de memoria a largo plazo.
        // Hay iterar por los bloques de 'zone' y si todos son libres,
        // desenlazar 'zone' de la lista (tiny_head o small_head) y llamar a munmap.
    }
}
