#define _GNU_SOURCE
#include "../incl/malloc.h"

void    *create_new_small_zone(void)
{
    t_zone          *zone;
    t_block         *block;
    size_t          aligned_size;
    size_t          total_size;
    unsigned char   *ptr;

    total_size = ((SMALL_MAX_SIZE + sizeof(t_block)) * BLOCKS_PER_ZONE);
    aligned_size = round_up_to_page_size(total_size);
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    zone = (t_zone *)ptr;
    block = (t_block *)(ptr + sizeof(t_zone));

    zone->head = block;
    zone->next = NULL;
    zone->total_size = aligned_size;

    separate_small_blocks(zone->head);

    if (!small_head)
        small_head = zone;
    else
        append_small_zone(&small_head, zone);
    return ((void *)(zone));
} 

void    *create_new_zone(size_t size)
{
    t_zone          *zone;
    t_block         *block;
    size_t          aligned_size;
    size_t          total_size;
    unsigned char   *ptr;

    total_size = ((TINY_MAX_SIZE + sizeof(t_block)) * BLOCKS_PER_ZONE);
    aligned_size = round_up_to_page_size(total_size);
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == NULL)
        return (NULL);
    
    zone = (t_zone *)ptr;
    block = (t_block *)(ptr + sizeof(t_zone));

    zone->head = block;
    zone->next = NULL;
    zone->total_size = aligned_size;

    block->size = aligned_size - sizeof(t_zone) - sizeof(t_block);
    block->is_free = true;
    block->next = NULL;

    if (!tiny_head)
        tiny_head = zone;
    else
        append_tiny_zone(&tiny_head, zone);
    return ((void *)(zone));
} 

void    *ft_malloc(size_t size)
{

    /* 1. Recibes una petición de size.
       2. Si no hay zona TINY, creas una nueva zona con un bloque gigante.
       3. DIVIDES ese bloque gigante en uno del tamaño size y el resto.
       4. Le das el bloque de size al usuario.
       5. Si ya hay una zona, buscas un bloque libre que sea lo suficientemente grande.
       6. Si lo encuentras, lo divides y se lo das al usuario.
       7. Si no hay espacio, creas una nueva zona y repites el proceso.
    */
    t_zone          *zone;
    t_block         *block;
    size_t          aligned_size;
    size_t          total_size;
    unsigned char   *ptr;

    if (size == 0)
        return (NULL);

    if (size <= TINY_MAX_SIZE)
    {
        printf("###### **** SOY UN TINY ***** #######\n");
        if (!tiny_head)
        {
            zone = create_new_zone(size);
            if (zone == NULL)
                return (NULL);
            
            block = find_and_split_block(tiny_head->head, size);
            if (block != NULL)
            {
                block->is_free = false;
                block->type = TINY;
                return ((void *)(block + 1));
            }
        }

        block = find_freeblocks_tiny_zones(zone->head, size);
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
            block->type = TINY;
            return((void *)(block + 1));
        }
    }
    else if (size <= SMALL_MAX_SIZE)
    {
        printf("###### **** SOY UN SMALL ***** #######\n");
        if (!small_head)
        {
            zone = create_new_small_zone();
            if (zone == NULL)
                return (NULL);
            block = zone->head;
            block->is_free = false;
            block->type = SMALL;
            return ((void *)(block + 1));
        }

        block = find_freeblocks_small_zones(small_head);

        if (block != NULL)
        {
            block->is_free = false;
            return((void*)(block + 1));
        }
        else
        {
            zone = create_new_small_zone();
            if (zone == NULL) 
                return (NULL);
            block = zone->head;
            block->is_free = false;
            block->type = SMALL;
            return ((void *)(block + 1));
        }
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
        block->type = LARGE;
        block->next = NULL;

        if (!large_head)
            large_head = zone;
        else
            append_zone(&large_head, zone);
        return ((void *)(block + 1));
    }
    return (NULL);
}
