#define _GNU_SOURCE
#include "../incl/malloc.h"
#include "../lib/printf/ft_printf.h"

size_t round_up_to_page_size(size_t total_size)
{
    size_t  page_size;
    size_t  num_pages;

    page_size = getpagesize();
    num_pages = (total_size + page_size - 1) / page_size;
    return (num_pages * page_size);
}

void    *create_new_zone(size_t len)
{
    t_zone          *new_zone;
    t_block         *block;
    size_t          aligned_size;
    size_t          total_size;
    unsigned char   *ptr;

    total_size = ((len + BLOCK_OFFSET) * BLOCKS_PER_ZONE);
    aligned_size = round_up_to_page_size(total_size);
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (ptr == MAP_FAILED)
    {
        print_str("Error: malloc failed to allocate [SIZE] bytes");
        return (NULL);
    }
    
    new_zone = (t_zone *)ptr;
    block = (t_block *)((char *)ptr + sizeof(t_zone));

    new_zone->head = block;
    new_zone->next = NULL;
    new_zone->total_size = aligned_size;

    block->size = aligned_size - sizeof(t_zone) - BLOCK_OFFSET;
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
    char            *env_var;

    if (size == 0)
        return (NULL);
    
    if (g_mutex_initialized == 0)
    {
        if (pthread_mutex_init(&g_malloc_mutex, NULL) != 0)
            return (NULL);
        g_mutex_initialized = 1;
    }

    env_var = getenv("MALLOC_VERBOSE");
    if (env_var != NULL)
        state = 1;

    pthread_mutex_lock(&g_malloc_mutex);
    
    if (size <= TINY_MAX_SIZE)
    {
        if (!tiny_head)
        {
            zone = create_new_zone(TINY_MAX_SIZE);
            if (!zone)
            {
                pthread_mutex_unlock(&g_malloc_mutex);
                return (NULL);
            }
        }
        block = find_and_split_block(tiny_head->head, size);
        
        if (!block)
        {
            zone = create_new_zone(TINY_MAX_SIZE);
            if (!zone)
            {
                pthread_mutex_unlock(&g_malloc_mutex);
                return (NULL);
            }
            block = find_and_split_block(tiny_head->head, size);
        }
        if (block)
        {
            block->is_free = false;
            block->type = TINY;
            if (state)
                ft_printf("DEBUG: Malloc en block TINY. Dirección Zone: %p, Dirección block: %p. Tamaño block: %u\n", zone,
                    (void *)((char *)block + BLOCK_OFFSET), (unsigned int)block->size);
            pthread_mutex_unlock(&g_malloc_mutex);
            return ((void *)((char *)block + BLOCK_OFFSET));
        }
        pthread_mutex_unlock(&g_malloc_mutex);
        return (NULL);
    }
    else if (size <= SMALL_MAX_SIZE)
    {
        if (!small_head)
        {
            zone = create_new_zone(SMALL_MAX_SIZE);
            if (!zone)
            {
                pthread_mutex_unlock(&g_malloc_mutex);
                return (NULL);
            }
        }

        block = find_and_split_block(small_head->head, size);
        if (!block)
        {
            zone = create_new_zone(SMALL_MAX_SIZE);
            if (!zone)
            {
                pthread_mutex_unlock(&g_malloc_mutex);
                return (NULL);
            }
            block = find_and_split_block(small_head->head, size);
        }
        if (block)
        {
            block->is_free = false;
            block->type = SMALL;
            if (state)
                ft_printf("DEBUG: Malloc en block SMALL. Dirección Zone: %p, Dirección block: %p. Tamaño block: %u\n", zone,
                    (void *)((char *)block + BLOCK_OFFSET), (unsigned int)block->size);
            pthread_mutex_unlock(&g_malloc_mutex);
            return ((void *)((char *)block + BLOCK_OFFSET));
        }
        pthread_mutex_unlock(&g_malloc_mutex);
        return (NULL);
    }
    else
    {
        total_size = size + BLOCK_OFFSET + sizeof(t_zone);
        aligned_size = round_up_to_page_size(total_size);

        ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (ptr == MAP_FAILED)
        {
            print_str("Error: malloc failed to allocate [SIZE] bytes");
            pthread_mutex_unlock(&g_malloc_mutex);
            return (NULL);
        } 
        
        zone = (t_zone *)ptr;
        block = (t_block *)(char *)(ptr + sizeof(t_zone));

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
        if (state)
            ft_printf("DEBUG: Malloc en block LARGE. Dirección Zone: %p, Dirección block: %p. Tamaño block: %u\n", zone,
                (void *)((char *)block + BLOCK_OFFSET), (unsigned int)block->size);
        pthread_mutex_unlock(&g_malloc_mutex);
        return ((void *)((char *)block + BLOCK_OFFSET));
    }
    return (NULL);
}