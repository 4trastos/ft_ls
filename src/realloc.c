#include "../incl/malloc.h"

/* 
1. Aumentar el tamaño:
    a). Si hay espacio adyacente:
    Si el bloque de memoria que quieres agrandar tiene 
    suficiente espacio libre inmediatamente después (en el siguiente bloque libre), 
    tu ft_realloc simplemente debe fusionar el bloque actual con ese espacio libre. 
    El puntero original que tenías sigue siendo válido. 
    
    b). Si no hay espacio:
    1. Asignar un nuevo bloque de memoria más grande, usando ft_malloc.
    2. Copiar los datos del bloque antiguo al nuevo bloque (usando ft_memcpy, por ejemplo).
    3. Liberar el bloque de memoria antiguo, usando ft_free.
    4. Devolver el puntero al nuevo bloque de memoria.
    
2. Disminuir el tamaño:
    a). Si el nuevo tamaño es menor que el tamaño actual, 
    tu ft_realloc debe ajustar el tamaño del bloque actual. 
    b).El exceso de memoria debe ser tratado como un nuevo bloque libre. 
    c).El puntero original sigue siendo válido.
*/

void    *ft_realloc(void *ptr, size_t size)
{
    t_zone *zone;
    t_block *data_block;

    if (!ptr)
        return;
    data_block = (t_block *)(char *)ptr - sizeof(t_block);
    printf("Dirección de data_block (realloc): %p\n", data_block);
    zone = find_zone_for_ptr(ptr);
    if (!zone)
        return;
    printf("Dirección de zone (realloc) : %p\n", zone);
}