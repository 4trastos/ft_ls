#ifndef MALLOC_H
# define MALLOC_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>

//*** struct functions ***

//*** explicit functions ***
void    *malloc(size_t size);
void    free(void *ptr);
void    *realloc(void *ptr, size_t size);

//*** auxiliary functions ***

#endif