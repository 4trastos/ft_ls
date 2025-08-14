#include "../incl/malloc.h"

void    show_alloc_mem(void)
{
    t_zone  *aux_zone;
    t_block *aux_block;
    void    *block_end;

    if (tiny_head != NULL)
    {
        printf("TINY [Start of the portion of RAM (zone) reserved by the system] : %p\n", (void*)tiny_head);
        aux_zone = tiny_head;
        while (aux_zone != NULL)
        {
            aux_block = aux_zone->head;
            while (aux_block != NULL)
            {
                block_end = (void *)aux_block + sizeof(t_block) + aux_block->size;
                printf("[Start of data block] %p - [End of data block] %p : [Block size] %zu bytes\n[Total Zone] : %zu bytes\n",
                        (void*)(aux_block + 1), block_end, aux_block->size, aux_zone->total_size);
                aux_block = aux_block->next;
            }
            aux_zone = aux_zone->next;
        }
    }

    if (small_head != NULL)
    {
        printf("SMALL [Start of the portion of RAM (zone) reserved by the system] : %p\n", (void*)small_head);
        aux_zone = small_head;
        while (aux_zone != NULL)
        {
            aux_block = aux_zone->head;
            while (aux_block != NULL)
            {
                block_end = (void *)aux_block + sizeof(t_block) + aux_block->size;
                printf("[Start of data block] %p - [End of data block] %p : [Block Size] %zu bytes\n[Total Zone] : %zu bytes\n",
                        (void*)(aux_block + 1), block_end, aux_block->size, aux_zone->total_size);
                aux_block = aux_block->next;
            }
            aux_zone = aux_zone->next;
        }
    }

    if (large_head != NULL)
    {
        printf("LARGE [Start of the portion of RAM (zone) reserved by the system] : %p\n", (void*)large_head);
        aux_zone = large_head;
        while (aux_zone != NULL)
        {
            aux_block = aux_zone->head;
            while (aux_block != NULL)
            {
                block_end = (void *)aux_block + sizeof(t_block) + aux_block->size;
                printf("[Start of data block] %p - [End of data block] %p : [Block size] %zu bytes\n[Total Zone] : %zu bytes\n",
                        (void*)(aux_block + 1), block_end, aux_block->size, aux_zone->total_size);
                aux_block = aux_block->next;
            }
            aux_zone = aux_zone->next;
        }
    }
}