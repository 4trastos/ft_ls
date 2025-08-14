#include "../incl/malloc.h"

int main(int argc, char **argv)
{
    char    *pointer_to_free[MAX_POINTERS];
    //size_t  size = 126;
    //size_t  size = 1020;
    //size_t  size = 6700;
    size_t  num_pointers = 0;
    size_t size = 0;
    size_t  i = 1;
    
    if (argc == 1)
    {
        write(1, "escribe algo ====>>>>> : \n", 26);
        return (1);
    }
    
    while (argv[i] && num_pointers < MAX_POINTERS)
    {
        size = ft_strlen(argv[i]) + 1;
        char *str = malloc(size);
        if (!str)
        {
            printf(" ERROR!!!!! FT_MALLOC NO FUNCIONA \n");
            return (1);
        }
        strcpy(str, argv[i]);

        pointer_to_free[num_pointers++] = str;

        printf ("\n1.  Mostrando memoria asignada : \n");
        printf ("\n");
        show_alloc_mem();
        printf("\n 2. ESTO ES QUE HEMOS GUARDADO EN LA MEMORIA ====>>>> : %s\n", str);
        i++;
    }


    printf("\n 3. PASAMOS A LIBERAR LA MEMORIA \n");

    i = 0;
    while (i < num_pointers)
    {
        free(pointer_to_free[i]);
        i++;
    }
    
    printf("\nMemoria liberada exitosamente.\n");

    return (0);
}