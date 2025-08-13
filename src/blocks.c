#include "../incl/malloc.h"

void    split_block(t_block *block, size_t size)
{
    t_block *new_block;

    if (block->size > size + sizeof(t_block))
    {
        // Calcular la dirección del nuevo bloque
        new_block = (t_block*)((char *)block + size + sizeof(t_block));

        // El tamaño del nuevo bloque es lo que nos queda.
        new_block->size = block->size - size - sizeof(t_block);
        new_block->is_free = true;

        // Enlazar el nuevo bloque a la lista
        new_block->next = block->next;

        // Actiualizar el bloque original para que apunte al nuevo.
        block->size = size;
        block->next = new_block;
    }
    return;
}

t_block *find_and_split_block(t_block *head, size_t size)
{
    t_block *aux;

    aux = head;
    while (aux != NULL)
    {
        if (aux->is_free == true && aux->size >= size)
        {
            split_block(aux, size);
            return(aux);
        }
        aux = aux->next;
    }
    return (NULL);
}
