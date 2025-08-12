#include "../incl/malloc.h"

int main(int argc, char **argv)
{
    char    *str;
    //(void)argc;
    //(void)argv;
    //size_t  size = 126;
    //size_t  size = 1020;
    //size_t  size = 6700;
    size_t size = ft_strlen(argv[1]) + 1;

    if (argc == 1)
    {
        write(1, "escribe algo ====>>>>> : \n", 26);
        return (1);
    }

    str = ft_malloc(size);
    if (!str)
    {
        printf(" ERROR!!!!! FT_MALLOC NO FUNCIONA \n");
        return (1);
    }
    strcpy(str, argv[1]);
    printf ("\n1.  Mostrando memoria asignada : \n");
    printf ("\n");
    show_alloc_mem();

    printf("\n 2. ESTO ES QUE HEMOS GUARDADO EN LA MEMORIA ====>>>> : %s\n", str);

    printf("\n 3. PASAMOS A LIBERAR LA MEMORIA \n");

    ft_free(str);
    return (0);
}