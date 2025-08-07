#include "../incl/malloc.h"

void    free(void *ptr)
{
    t_block *data_block;

    if (!ptr)
        return;
    
    // 1. Encontrar el bloque de memoria
    data_block = (t_block *)((char *)ptr - sizeof(t_block));
    
    // 2. Marcar el bloque como libre
    data_block->is_free = true;
    
    // 3. Fusión de bloques adyacentes
}