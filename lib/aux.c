#include "../incl/malloc.h"

size_t  ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

void    append_zone(t_zone **large_head, t_zone *zone)
{
    t_zone  *aux;
    aux = *large_head;
    while (aux->next)
        aux = aux->next;
     aux->next = zone;
}