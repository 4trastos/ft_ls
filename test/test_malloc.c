#include "../incl/malloc.h"
#include "../lib/printf/ft_printf.h"

int main(int argc, char **argv)
{
    char *str;
    size_t  len;

    if (argc < 2)
    {
        ft_printf("Escribe el número de bytes : ==>>> ./test_malloc_app <bytes>\n");
        return (1);
    }
    len = atoi(argv[1]);

    str = malloc(len);
    show_alloc_mem();

    //ft_printf("##### REALLOC CON 25bytes más #####\n");
    str = realloc(str, (len + 25));
    show_alloc_mem();

    //ft_printf("##### LIBERAMOS ELPUNTERO #####\n");
    free(str);
    //ft_printf("##### LIBERAMOS EL MISMO PUNTERO #####\n");
    //free(str);

    return (0);
}

/* #define TEST_ARRAY_SIZE 10000
#define NUM_THREADS 5

static void ft_memset(void *s, int c, size_t n) {
    char *ptr = (char *)s;
    while (n--) {
        *ptr++ = (char)c;
    }
}

// Función que será ejecutada por cada hilo
void *thread_test_func(void *arg) {
    char *ptrs[100];
    //int i;

    // Realizar un bucle de asignaciones y liberaciones aleatorias
    for (int j = 0; j < 100; j++) {
        // Asignar un bloque de tamaño aleatorio
        ptrs[j] = malloc(rand() % 2048 + 1);
        if (ptrs[j]) {
            // Llenar el bloque con un valor para verificar la integridad
            ft_memset(ptrs[j], 'A' + (long)arg, rand() % 2048 + 1);
        }
    }
    
    // Reasignar la mitad de los bloques a un nuevo tamaño
    for (int k = 0; k < 100; k++) {
        if (ptrs[k] && rand() % 2 == 0) {
            ptrs[k] = realloc(ptrs[k], rand() % 3000 + 1);
            if (ptrs[k]) {
                 ft_memset(ptrs[k], 'B' + (long)arg, rand() % 3000 + 1);
            }
        }
    }

    // Liberar todos los bloques
    for (int l = 0; l < 100; l++) {
        if (ptrs[l]) {
            free(ptrs[l]);
        }
    }
    ft_printf("Hilo %d completado.\n", (long)arg);
    return NULL;
}


int main(int argc, char **argv) {
    if (argc < 2) {
        ft_printf("Uso: ./test_scenarios <numero_de_prueba>\n");
        ft_printf("1: Asignaciones/liberaciones aleatorias (fragmentacion)\n");
        ft_printf("2: malloc(0) y free(NULL)\n");
        ft_printf("3: Doble free (corrupcion)\n");
        ft_printf("4: Uso despues de free (segfault)\n");
        ft_printf("5: Malloc LARGE\n");
        ft_printf("6: Realloc con valores limite\n");
        ft_printf("7: Realloc para crecer en su lugar\n");
        ft_printf("8: Realloc para encogerse y dividir el bloque\n");
        ft_printf("9: Realloc de SMALL a LARGE y viceversa\n");
        ft_printf("10: Llenado y liberacion selectiva de memoria\n");
        ft_printf("11: Test de estres multihilo\n");
        return 1;
    }

    int test_num = atoi(argv[1]);
    char *ptr1, *ptr2;
    size_t i;

    switch (test_num) {
        case 1:
            // Test 1: Asignaciones y liberaciones aleatorias (fragmentación)
            ft_printf("--- Ejecutando Test 1: Malloc/Free aleatorios ---\n");
            char *ptr_array[TEST_ARRAY_SIZE];
            ft_printf("Realizando %d asignaciones...\n", TEST_ARRAY_SIZE);
            for (i = 0; i < TEST_ARRAY_SIZE; i++) {
                ptr_array[i] = malloc(i + 1); // Tamaños incrementales
            }
            ft_printf("Liberando la mitad de la memoria para crear fragmentacion...\n");
            for (i = 0; i < TEST_ARRAY_SIZE; i += 2) {
                free(ptr_array[i]);
            }
            show_alloc_mem();
            ft_printf("Liberando el resto de la memoria.\n");
            for (i = 1; i < TEST_ARRAY_SIZE; i += 2) {
                free(ptr_array[i]);
            }
            ft_printf("\nTest 1 completado. Si no hubo crashes y la salida de show_alloc_mem es coherente, ¡tu malloc es robusto!\n");
            break;
        case 2:
            // Test 2: malloc(0) y free(NULL)
            ft_printf("--- Ejecutando Test 2: malloc(0) y free(NULL) ---\n");
            ptr1 = malloc(0);
            show_alloc_mem();
            ft_printf("malloc(0) devolvio: %p\n", (void*)ptr1);
            show_alloc_mem();
            ft_printf("Llamando a free(NULL)...\n");
            show_alloc_mem();
            free(NULL); // No debería hacer nada
            ft_printf("Llamando a free en el puntero de malloc(0)...\n");
            free(ptr1); 
            ft_printf("Test 2 completado. Si no hubo crashes ni mensajes de error, el comportamiento es correcto.\n");
            break;
        case 3:
            // Test 3: Doble free
            ft_printf("--- Ejecutando Test 3: Doble free ---\n");
            ptr1 = malloc(50);
            show_alloc_mem();
            ft_printf("Asignado un bloque y lo liberamos.\n");
            free(ptr1);
            ft_printf("Intentando liberar el mismo puntero de nuevo...\n");
            free(ptr1); // Esto debería causar un error
            ft_printf("Test 3 completado. (No se tiene que ver este mensaje).\n");
            break;
        case 4:
            // Test 4: Uso despues de free
            ft_printf("--- Ejecutando Test 4: Uso despues de free ---\n");
            ptr1 = malloc(20);
            show_alloc_mem();
            strcpy(ptr1, "Hola mundo!");
            ft_printf("Cadena guardada: %s\n", ptr1);
            free(ptr1);
            ft_printf("Intentando escribir en la memoria despues de liberarla...\n");
            strcpy(ptr1, "Error!"); // Esto debería causar un segfault
            ft_printf("Test 4 completado (No se tiene que ver este mensaje).\n");
            break;
        case 5:
            // Test 5: Malloc LARGE
            ft_printf("--- Ejecutando Test 5: Malloc LARGE ---\n");
            ptr1 = malloc(1000000); // 1 MB
            show_alloc_mem();
            if (ptr1) {
                ft_printf("Asignado un bloque LARGE. Direccion: %p\n", (void*)ptr1);
                show_alloc_mem();
                free(ptr1);
                ft_printf("Test 5 completado. Si el bloque se muestra y luego se libera, es correcto.\n");
            } else {
                ft_printf("Fallo al asignar un bloque LARGE.\n");
            }
            break;
        case 6:
            // Test 6: Realloc con valores limite
            ft_printf("--- Ejecutando Test 6: Realloc con valores limite ---\n");
            ft_printf("Probando realloc(NULL, 50)...\n");
            ptr1 = realloc(NULL, 50);
            if (ptr1)
            {
                ft_printf("realloc(NULL, 50) OK. Direccion: %p\n", (void*)ptr1);
                free(ptr1);
            }
            ft_printf("Probando realloc(ptr, 0)...\n");
            ptr1 = malloc(50);
            ptr2 = realloc(ptr1, 0);
            ft_printf("realloc(ptr, 0) OK. El puntero final es: %p (debe ser NULL)\n", (void*)ptr2);
            ft_printf("Probando realloc(ptr, mismo_tamaño)...\n");
            ptr1 = malloc(50);
            ptr2 = realloc(ptr1, 50);
            ft_printf("realloc(ptr, 50) OK. La direccion final es: %p (debe ser la misma que %p)\n", (void*)ptr2, (void*)ptr1);
            show_alloc_mem();
            free(ptr2);
            ft_printf("Test 6 completado. Si el comportamiento coincide, es correcto.\n");
            break;
        case 7:
            // Test 7: Realloc para crecer en su lugar (coalescencia)
            ft_printf("--- Ejecutando Test 7: Realloc para crecer en su lugar ---\n");
            ptr1 = malloc(50); // Primer bloque
            show_alloc_mem();
            ptr2 = malloc(50); // Segundo bloque
            show_alloc_mem();
            free(ptr2);
            ft_printf("Asignamos dos bloques, liberamos el segundo. Reasignando el primero para que crezca...\n");
            ptr1 = realloc(ptr1, 120); // Debería caber en el espacio de 50 + 50
            show_alloc_mem();
            ft_printf("Dirección del puntero después de realloc: %p (debería ser la misma que la original)\n", (void*)ptr1);
            free(ptr1);
            ft_printf("Test 7 completado. Si el puntero no ha cambiado, el crecimiento en su lugar funciona.\n");
            break;
        case 8:
            // Test 8: Realloc para encogerse y dividir el bloque
            ft_printf("--- Ejecutando Test 8: Realloc para encogerse ---\n");
            ptr1 = malloc(200);
            show_alloc_mem();
            ft_printf("Asignamos un bloque grande (200 bytes). Ahora lo reducimos a 50 bytes.\n");
            ptr2 = realloc(ptr1, 50);
            show_alloc_mem();
            ft_printf("Dirección del puntero después de realloc: %p (debería ser la misma que la original)\n", (void*)ptr2);
            free(ptr2);
            ft_printf("Test 8 completado. show_alloc_mem debería mostrar un bloque libre resultante.\n");
            break;
        case 9:
            // Test 9: Realloc entre zonas
            ft_printf("--- Ejecutando Test 9: Realloc entre zonas ---\n");
            // Caso 1: SMALL a LARGE
            ptr1 = malloc(100); // Bloque SMALL
            show_alloc_mem();
            ft_memset(ptr1, 'A', 100);
            show_alloc_mem();
            ft_printf("Reasignando de SMALL a LARGE...\n");
            ptr2 = realloc(ptr1, 100000); // 100 KB
            show_alloc_mem();
            ft_printf("Puntero original (SMALL): %p | Nuevo puntero (LARGE): %p\n", (void*)ptr1, (void*)ptr2);
            show_alloc_mem();
            free(ptr2);

            // Caso 2: LARGE a SMALL
            ptr1 = malloc(100000); // Bloque LARGE
            show_alloc_mem();
            ft_memset(ptr1, 'B', 100000);
            ft_printf("Reasignando de LARGE a SMALL...\n");
            ptr2 = realloc(ptr1, 100);
            show_alloc_mem();
            ft_printf("Puntero original (LARGE): %p | Nuevo puntero (SMALL): %p\n", (void*)ptr1, (void*)ptr2);
            free(ptr2);
            ft_printf("Test 9 completado. Si los punteros cambian y no hay crashes, el realloc entre zonas funciona.\n");
            break;
        case 10:
            // Test 10: Llenado de memoria y liberaciones selectivas
            ft_printf("--- Ejecutando Test 10: Llenado de memoria y liberaciones selectivas ---\n");
            char *chunks[500];
            ft_printf("Llenando la memoria con bloques de 16 bytes...\n");
            for (i = 0; i < 500; i++) {
                chunks[i] = malloc(16);
            }
            show_alloc_mem();
            ft_printf("Liberando bloques alternos...\n");
            for (i = 0; i < 500; i += 2) {
                free(chunks[i]);
            }
            ft_printf("show_alloc_mem despues de liberar la mitad:\n");
            show_alloc_mem();
            ft_printf("Liberando el resto de los bloques.\n");
            for (i = 1; i < 500; i += 2) {
                free(chunks[i]);
            }
            ft_printf("show_alloc_mem despues de liberar todos los bloques:\n");
            show_alloc_mem();
            ft_printf("Test 10 completado. Si la memoria se ha liberado por completo al final, la coalescencia funciona.\n");
            break;
        case 11:
            // Test 11: Test de estres multihilo
            ft_printf("--- Ejecutando Test 11: Test de estres multihilo ---\n");
            ft_printf("Creando %d hilos para realizar operaciones aleatorias.\n", NUM_THREADS);
            pthread_t threads[NUM_THREADS];
            for (i = 0; i < NUM_THREADS; i++) {
                if (pthread_create(&threads[i], NULL, thread_test_func, (void *)(long)i) != 0) {
                    ft_printf("Error al crear el hilo %zu\n", i);
                    return 1;
                }
            }
            for (i = 0; i < NUM_THREADS; i++) {
                pthread_join(threads[i], NULL);
            }
            ft_printf("Todos los hilos han terminado. show_alloc_mem despues de todas las operaciones:\n");
            show_alloc_mem();
            ft_printf("Test 11 completado. Si no hubo crashes, deadlocks, o corrupcion de memoria, tu solucion multihilo es robusta.\n");
            break;
        default:
            ft_printf("Numero de test no valido.\n");
            break;
    }
    return 0;
} */