#ifndef MALLOC_H
# define MALLOC_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/mman.h>
# include <errno.h>

# define TINY_MAX_SIZE  128
# define SMALL_MAX_SIZE 1024

# define BLOCKS_PER_ZONE 100

typedef enum
{
    TINY,
    SMALL,
    LARGE
}   e_zone_type;

typedef struct s_block
{
    size_t          size;    
    bool            is_free;
    e_zone_type     type;
    struct s_block  *next;
}   t_block;

typedef struct s_zone
{
    t_block         *head;
    size_t          total_size;
    struct s_zone   *next; 
}   t_zone;

extern t_zone       *tiny_head;
extern t_zone       *small_head;                         
extern t_zone       *large_head;

//*** struct functions ***
void    append_zone(t_zone **large_head, t_zone *zone);
void    append_small_zone(t_zone **small_head, t_zone *zone);
void    append_tiny_zone(t_zone **tiny_head, t_zone *zone);

//*** explicit functions ***
void    *ft_malloc(size_t size);
void    ft_free(void *ptr);
void    *ft_realloc(void *ptr, size_t size);
void    show_alloc_mem(void);
void    separate_blocks(t_block *list);
void    separate_small_blocks(t_block *list);
void    *find_free_block(t_block *block);
void    *find_free_small_block(t_block *block);
void    *create_new_zone(size_t size);
void    split_block(t_block *block, size_t size);
size_t  round_up_to_page_size(size_t size);
t_block *find_freeblocks_small_zones(t_zone *head);
t_block *find_freeblocks_tiny_zones(t_block *block, size_t size);

//*** auxiliary functions ***
size_t  ft_strlen(char *str);
void    print_str(char *str);

#endif