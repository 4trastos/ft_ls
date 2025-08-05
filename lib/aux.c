#include "../incl/malloc.h"

size_t round_up_to_page_size(size_t size)
{
    // 1. Obtener el tamaño de la página del sistema
    size_t page_size = getpagesize();

    // 2. Calcular el número de páginas necesarias
    //    Esto se puede hacer con una división y un redondeo, o con aritmética de bits.
    size_t num_pages = (size + page_size - 1) / page_size;

    // 3. Devolver el tamaño redondeado
    return num_pages * page_size;
}