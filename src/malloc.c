#define _GNU_SOURCE
#include "../incl/malloc.h"

size_t round_up_to_page_size(size_t size)
{
    size_t  page_size;
    size_t  num_pages;

    page_size = getpagesize();
    num_pages = (size + page_size - 1) / page_size;

    return (num_pages * page_size);
}

void    *create_new_zone(size_t len)
{
    t_zone          *new_zone;
    t_block         *block;
    size_t          aligned_size;
    size_t          total_size;
    unsigned char   *ptr;

    total_size = ((len + sizeof(t_block)) * BLOCKS_PER_ZONE);
    aligned_size = round_up_to_page_size(total_size);
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED)
    {
        print_str("Error: malloc failed to allocate [SIZE] bytes");
        return (NULL);
    }
    
    new_zone = (t_zone *)ptr;
    block = (t_block *)(ptr + sizeof(t_zone));

    new_zone->head = block;
    new_zone->next = NULL;
    new_zone->total_size = aligned_size;

    block->size = aligned_size - sizeof(t_zone) - sizeof(t_block);
    block->is_free = true;
    block->next = NULL;
    block->prev = NULL;
    block->type = (len == TINY_MAX_SIZE) ? TINY : SMALL;

    if (len == TINY_MAX_SIZE)
    {
        if (!tiny_head)
            tiny_head = new_zone;
        else
            append_tiny_zone(&tiny_head, new_zone);
    }
    else
    {
        if (!small_head)
            small_head = new_zone;
        else
            append_small_zone(&small_head, new_zone);
    }
    return ((void *)(new_zone));
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
        if (!tiny_head)
        {
            zone = create_new_zone(TINY_MAX_SIZE);
            if (!zone)
                return (NULL);
        }
            
        block = find_and_split_block(tiny_head->head, size);
        printf("Dirección de zone       (malloc)  : %p\n", zone);
        printf("Dirección de block      (malloc)  : %p\n", block);
        printf("Tamaño de bytes         (malloc)  : %ld\n", size);

        if (!block)
        {
            zone = create_new_zone(TINY_MAX_SIZE);
            if (!zone)
                return (NULL);
            block = find_and_split_block(tiny_head->head, size);
        }
        if (block)
        {
            block->is_free = false;
            block->type = TINY;
            return ((void *)(block + 1));
        }
        return (NULL);
    }
    else if (size <= SMALL_MAX_SIZE)
    {
        if (!small_head)
        {
            zone = create_new_zone(SMALL_MAX_SIZE);
            if (!zone)
                return (NULL);
        }

        block = find_and_split_block(small_head->head, size);
        if (!block)
        {
            zone = create_new_zone(SMALL_MAX_SIZE);
            if (!zone)
                return (NULL);
            block = find_and_split_block(small_head->head, size);
        }
        if (block)
        {
            block->is_free = false;
            block->type = SMALL;
            return ((void *)(block + 1));
        }
        return (NULL);
    }
    else
    {
        total_size = size + sizeof(t_block) + sizeof(t_zone);
        aligned_size = round_up_to_page_size(total_size);

        ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (ptr == MAP_FAILED)
        {
            print_str("Error: malloc failed to allocate [SIZE] bytes");
            return (NULL);
        } 
        
        zone = (t_zone *)ptr;
        block = (t_block *)(ptr + sizeof(t_zone));

        zone->head = block;
        zone->total_size = aligned_size;
        zone->next = NULL;

        block->is_free = false;
        block->size = size;
        block->type = LARGE;
        block->next = NULL;
        block->prev = NULL;

        if (!large_head)
            large_head = zone;
        else
            append_zone(&large_head, zone);
        return ((void *)(block + 1));
    }
    return (NULL);
}