#define _GNU_SOURCE
#include "../incl/malloc.h"

void    separate_blocks(t_block *list)
{
    t_block *aux;
    t_block *next_block;

    aux = list;
    for (size_t i = 0; i < (BLOCKS_PER_ZONE -1); i++)
    {
        aux->size = TINY_MAX_SIZE;
        aux->is_free = true;

        next_block = (t_block*)((char *)aux + TINY_MAX_SIZE + sizeof(t_block));
        aux->next = next_block;
        aux = next_block;
    }
    aux->size = TINY_MAX_SIZE;
    aux->is_free = true;
    aux->next = NULL;
}

void   *find_free_block(t_block *block)
{
    t_block *aux;

    aux = block;
    while (aux != NULL)
    {
        if (aux->is_free == true)
            return(aux);
        aux = aux->next;
    }
    return (NULL);
}

void    *create_new_zone(size_t size)
{
    t_zone          *zone;
    t_block         *block;
    size_t          aligned_size;
    size_t          total_size;
    unsigned char   *ptr;

    total_size = (TINY_MAX_SIZE + sizeof(t_block) * BLOCKS_PER_ZONE);
    aligned_size = round_up_to_page_size(total_size);
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    zone = (t_zone *)ptr;
    block = (t_block *)(ptr + sizeof(t_zone));

    zone->head = block;
    zone->next = NULL;
    zone->total_size = aligned_size;

    block->size = TINY_MAX_SIZE;
    block->next = NULL;

    separate_blocks(zone->head);

    if (!tiny_head)
        tiny_head = zone;
    else
        append_zone(&tiny_head, zone);
    return ((void *)(zone));
} 

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
        block = find_free_block(tiny_head->head);

        if (block != NULL)
        {
            block->is_free = false;
            return ((void *)(block + 1));
        }
        else
        {
            zone = create_new_zone(size);
            if (zone == NULL)
                return (NULL);
            block = zone->head;
            block->is_free = false;
        }
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
