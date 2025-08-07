#include "../incl/malloc.h"

void show_alloc_mem(void)
{
    t_zone  *aux_zone;
    t_block *aux_block;
    void    *block_end;

    if (large_head != NULL)
    {
        printf("LARGE : %p\n", (void*)large_head);
        aux_zone = large_head;
        while (aux_zone != NULL)
        {
            aux_block = aux_zone->head;
            while (aux_block != NULL)
            {
                block_end = (void *)aux_block + sizeof(t_block) + aux_block->size;
                printf("%p - %p : %zu octets (Total Zona: %zu)\n", 
                       (void *)(aux_block + 1), block_end, aux_block->size, aux_zone->total_size);
                aux_block = aux_block->next;
            }
            aux_zone = aux_zone->next;
        }
    }
}

size_t round_up_to_page_size(size_t size)
{
    size_t  page_size;
    size_t  num_pages;

    page_size = getpagesize();
    num_pages = (size + page_size - 1) / page_size;

    return (num_pages * page_size);
}