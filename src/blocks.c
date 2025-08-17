#include "../incl/malloc.h"
#include "../lib/ft_printf/ft_printf.h"

void    split_block(t_block *block, size_t size)
{
    t_block *new_block;

    size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);

    if (block->size > size + BLOCK_OFFSET)
    {
        // Calcular la dirección del nuevo bloque
        new_block = (t_block*)((char *)block + size + BLOCK_OFFSET);

        // El tamaño del nuevo bloque es lo que nos queda.
        new_block->size = block->size - size - BLOCK_OFFSET;
        new_block->is_free = true;
        new_block->type = block->type;

        // Enlazar el nuevo bloque a la lista
        new_block->next = block->next;

        if (new_block->next != NULL)
            new_block->next->prev = new_block;
        
        block->size = size;
        block->next = new_block;
        new_block->prev = block;
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
