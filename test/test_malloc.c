#include "../incl/malloc.h"

int main(void)
{
    char    *str;
    //size_t  size = 126;
    //size_t  size = 1020;
    size_t  size = 6700;

    /* if (argc == 1)
    {
        write(1, "escribe algo ====>>>>> : \n", 26);
        return (1);
    } */

    //str = ft_malloc(ft_strlen(argv[1]) + 1);
    str = ft_malloc(size);
    if (!str)
    {
        printf(" ERROR!!!!! FT_MALLOC NO FUNCIONA \n");
        return (1);
    }
    printf ("\n**** Mostrando memoria asignada ****\n");
    printf ("\n");
    show_alloc_mem();

    printf("\n ESTO ES QUE HEMOS GUARDADO EN LA MEMORIA ====>>>> : %s\n", str);
    printf("\n **** MALLOC FUNCIONA PERFECTAMENTE ******** \n");
    
    return (0);
}