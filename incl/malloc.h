#ifndef MALLOC_H
# define MALLOC_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>
# include <sys/mman.h>

# define TINY_MAX_SIZE  128
# define SMALL_MAX_SIZE 1024

# define BLOCKS_PER_ZONE    100

// El tamaño de la zona se calcula como (tamaño_max + sizeof(t_block)) * num_bloques
// Esto lo puedes hacer en tiempo de ejecución para no definir una constante.
// size_t tiny_zone_size = (TINY_MAX_SIZE + sizeof(t_block)) * BLOCKS_PER_ZONE;
// size_t small_zone_size = (SMALL_MAX_SIZE + sizeof(t_block)) * BLOCKS_PER_ZONE;

// Esta estructura va al principio de cada bloque de memoria asignado (o libre).
// Contendrá la información necesaria para gestionar el bloque.

typedef struct s_block
{
    size_t              size;       //Tamaño del bloque de memoria (sin incluir el tamaño de la propia struct)
    bool                is_free;    //Si el bloque está libre o no.
    struct s_block      *next;
}   t_block;

//  Esta estructura gestionará cada zona de memoria que asignes con mmap.

typedef struct s_zone
{
    t_block         *head;          // Puntero al primer bloque de la memoria en esta zona
    size_t          total_size;     // Tamaño total de la zona asignada por nmap.
    struct s_zone   *next;          // Puntero a la siguiente zona de memoria del mismo tipo (TINY, SMALL, LARGE)

}   t_zone;

t_zone *tiny_head;
t_zone *small_head;
t_zone *large_head;

//*** struct functions ***

//*** explicit functions ***
void    *ft_malloc(size_t size);
void    ft_free(void *ptr);
void    *ft_realloc(void *ptr, size_t size);
void    show_alloc_mem(void);

//*** auxiliary functions ***
size_t    round_up_to_page_size(size_t size);

#endif