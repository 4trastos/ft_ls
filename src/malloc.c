#define _GNU_SOURCE
#include "../incl/malloc.h"

int    find_free_block();   //  recorra las zonas existentes (usando tiny_head o small_head).
void    create_new_zone();  //  Si la búsqueda falla.  Esta función calculará el tamaño de la nueva zona, la redondeará al tamaño de página y llamará a mmap para asignarla.

void    *malloc(size_t size)
{
    t_zone          *zone;
    t_block         *block;
    size_t          aligned_size;
    size_t          total_size;
    unsigned char   *ptr;

    if (size == 0)
        return (NULL);

    if (size <= TINY_MAX_SIZE)
    {
        total_size = (TINY_MAX_SIZE + sizeof(t_block) * BLOCKS_PER_ZONE);
        aligned_size = round_up_to_page_size(total_size);
        ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    }
    else if (size <= SMALL_MAX_SIZE)
    {
        total_size = (SMALL_MAX_SIZE + sizeof(t_block) * BLOCKS_PER_ZONE);
        aligned_size = round_up_to_page_size(total_size);
        ptr = mmap(NULL, aligned_alloc, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    }
    else
    {
        total_size = size + sizeof(t_block) + sizeof(t_zone);
        aligned_size = round_up_to_page_size(total_size);

        ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (ptr == MAP_FAILED)
            return (NULL);
        
        zone = (t_zone *)ptr;
        block = (t_block *)(ptr + sizeof(t_zone));

        zone->head = block;
        zone->total_size = aligned_size;
        zone->next = NULL;

        block->is_free = false;
        block->size = size;
        block->next = NULL;

        if (!large_head)
            large_head = zone;
        else
            append_zone(&large_head, zone);
        return ((void *)(block + 1));

    }
    return (NULL);
}
