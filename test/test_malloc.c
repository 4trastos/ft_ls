#include "../incl/malloc.h"

int main(void)
{
    char *str;
    size_t  size_to_test = 2000;

    /* if (argc == 1)
    {
        write(1, "escribe algo ====>>>>> : \n", 26);
        return (1);
    } */

    //str = ft_malloc(ft_strlen(argv[1]) + 1);
    str = ft_malloc(size_to_test);
    if (!str)
    {
        printf(" ERROR!!!!! FT_MALLOC NO FUNCIONA \n");
        return (1);
    }
    printf ("\n**** Mostrando memoria asignada ****\n");
    show_alloc_mem();

    printf("ESTO ES QUE HEMOS GUARDADO EN LA MEMORIA ====>>>> : %s", str);
    printf("\n **** MALLOC FUNCIONA PERFECTAMENTE ******** \n");
    
    return (0);
}