#include "../incl/malloc.h"
#include "../lib/ft_printf/ft_printf.h"

void    show_alloc_mem(void)
{
    t_zone  *aux_zone;
    t_block *aux_block;
    void    *block_start;
    void    *block_end;
    size_t  total_bytes;

    total_bytes = 0;
    if (tiny_head != NULL)
    {
        ft_printf("TINY : %p\n", (void*)tiny_head);
        aux_zone = tiny_head;
        while (aux_zone != NULL)
        {
            aux_block = aux_zone->head;
            while (aux_block != NULL)
            {
                block_start = (void *)((char *)aux_block + BLOCK_OFFSET);
                block_end = (void *)((char *)aux_block + BLOCK_OFFSET + aux_block->size);

                ft_printf("%p - %p : %d bytes\n", block_start, block_end, aux_block->size);
                total_bytes = total_bytes + aux_block->size;
                aux_block = aux_block->next;
            }
            aux_zone = aux_zone->next;
        }
        ft_printf("Total : %d bytes\n", total_bytes);
    }

    total_bytes = 0;
    if (small_head != NULL)
    {
        ft_printf("SMALL : %p\n", (void*)small_head);
        aux_zone = small_head;
        while (aux_zone != NULL)
        {
            aux_block = aux_zone->head;
            while (aux_block != NULL)
            {
                block_start = (void *)((char *)aux_block + BLOCK_OFFSET);
                block_end = (void *)((char *)aux_block + BLOCK_OFFSET + aux_block->size);

                ft_printf("%p - %p : %d bytes\n", block_start, block_end, aux_block->size);
                total_bytes = total_bytes + aux_block->size;
                aux_block = aux_block->next;
            }
            aux_zone = aux_zone->next;
        }
        ft_printf("Total : %d bytes\n", total_bytes);
    }

    total_bytes = 0;
    if (large_head != NULL)
    {
        ft_printf("LARGE : %p\n", (void*)large_head);
        aux_zone = large_head;
        while (aux_zone != NULL)
        {
            aux_block = aux_zone->head;
            while (aux_block != NULL)
            {
                block_start = (void *)((char *)aux_block + BLOCK_OFFSET);
                block_end = (void *)((char *)aux_block + BLOCK_OFFSET + aux_block->size);

                ft_printf("%p - %p : %d bytes\n", block_start, block_end, aux_block->size);
                total_bytes = total_bytes + aux_block->size;
                aux_block = aux_block->next;
            }
            aux_zone = aux_zone->next;
        }
        ft_printf("Total : %d bytes\n", total_bytes);
    }
}