#include "../incl/malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_ARRAY_SIZE 1000

static void ft_memset(void *s, int c, size_t n) {
    char *ptr = (char *)s;
    while (n--) {
        *ptr++ = (char)c;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: ./test_scenarios <numero_de_prueba>\n");
        printf("1: Asignaciones/liberaciones aleatorias (fragmentacion)\n");
        printf("2: malloc(0) y free(NULL)\n");
        printf("3: Doble free (corrupcion)\n");
        printf("4: Uso despues de free (segfault)\n");
        printf("5: Malloc LARGE\n");
        printf("6: Realloc con valores limite\n");
        printf("7: Realloc para crecer en su lugar\n");
        printf("8: Realloc para encogerse y dividir el bloque\n");
        printf("9: Realloc de SMALL a LARGE y viceversa\n");
        printf("10: Llenado y liberacion selectiva de memoria\n");
        return 1;
    }

    int test_num = atoi(argv[1]);
    char *ptr1, *ptr2;
    size_t i;

    switch (test_num) {
        case 1:
            // Test 1: Asignaciones y liberaciones aleatorias (fragmentación)
            printf("--- Ejecutando Test 1: Malloc/Free aleatorios ---\n");
            char *ptr_array[TEST_ARRAY_SIZE];
            printf("Realizando %d asignaciones...\n", TEST_ARRAY_SIZE);
            for (i = 0; i < TEST_ARRAY_SIZE; i++) {
                ptr_array[i] = malloc(i + 1); // Tamaños incrementales
            }
            printf("Liberando la mitad de la memoria para crear fragmentacion...\n");
            for (i = 0; i < TEST_ARRAY_SIZE; i += 2) {
                free(ptr_array[i]);
            }
            printf("Liberando el resto de la memoria.\n");
            for (i = 1; i < TEST_ARRAY_SIZE; i += 2) {
                free(ptr_array[i]);
            }
            show_alloc_mem();
            printf("\nTest 1 completado. Si no hubo crashes y la salida de show_alloc_mem es coherente, ¡tu malloc es robusto!\n");
            break;
        case 2:
            // Test 2: malloc(0) y free(NULL)
            printf("--- Ejecutando Test 2: malloc(0) y free(NULL) ---\n");
            ptr1 = malloc(0);
            printf("malloc(0) devolvio: %p\n", (void*)ptr1);
            printf("Llamando a free(NULL)...\n");
            free(NULL); // No debería hacer nada
            printf("Llamando a free en el puntero de malloc(0)...\n");
            free(ptr1); 
            printf("Test 2 completado. Si no hubo crashes ni mensajes de error, el comportamiento es correcto.\n");
            break;
        case 3:
            // Test 3: Doble free
            printf("--- Ejecutando Test 3: Doble free ---\n");
            ptr1 = malloc(50);
            printf("Asignado un bloque y lo liberamos.\n");
            free(ptr1);
            printf("Intentando liberar el mismo puntero de nuevo...\n");
            free(ptr1); // Esto debería causar un error
            printf("Test 3 completado. (No se tiene que ver este mensaje).\n");
            break;
        case 4:
            // Test 4: Uso despues de free
            printf("--- Ejecutando Test 4: Uso despues de free ---\n");
            ptr1 = malloc(20);
            strcpy(ptr1, "Hola mundo!");
            printf("Cadena guardada: %s\n", ptr1);
            free(ptr1);
            printf("Intentando escribir en la memoria despues de liberarla...\n");
            strcpy(ptr1, "Error!"); // Esto debería causar un segfault
            printf("Test 4 completado (No se tiene que ver este mensaje).\n");
            break;
        case 5:
            // Test 5: Malloc LARGE
            printf("--- Ejecutando Test 5: Malloc LARGE ---\n");
            ptr1 = malloc(1000000); // 1 MB
            if (ptr1) {
                printf("Asignado un bloque LARGE. Direccion: %p\n", (void*)ptr1);
                show_alloc_mem();
                free(ptr1);
                printf("Test 5 completado. Si el bloque se muestra y luego se libera, es correcto.\n");
            } else {
                printf("Fallo al asignar un bloque LARGE.\n");
            }
            break;
        case 6:
            // Test 6: Realloc con valores limite
            printf("--- Ejecutando Test 6: Realloc con valores limite ---\n");
            printf("Probando realloc(NULL, 50)...\n");
            ptr1 = realloc(NULL, 50);
            if (ptr1) {
                printf("realloc(NULL, 50) OK. Direccion: %p\n", (void*)ptr1);
                free(ptr1);
            }
            printf("Probando realloc(ptr, 0)...\n");
            ptr1 = malloc(50);
            ptr2 = realloc(ptr1, 0);
            printf("realloc(ptr, 0) OK. El puntero final es: %p (debe ser NULL)\n", (void*)ptr2);
            printf("Probando realloc(ptr, mismo_tamaño)...\n");
            ptr1 = malloc(50);
            ptr2 = realloc(ptr1, 50);
            printf("realloc(ptr, 50) OK. La direccion final es: %p (debe ser la misma que %p)\n", (void*)ptr2, (void*)ptr1);
            free(ptr2);
            printf("Test 6 completado. Si el comportamiento coincide, es correcto.\n");
            break;
        case 7:
            // Test 7: Realloc para crecer en su lugar (coalescencia)
            printf("--- Ejecutando Test 7: Realloc para crecer en su lugar ---\n");
            ptr1 = malloc(50); // Primer bloque
            ptr2 = malloc(50); // Segundo bloque
            free(ptr2);
            printf("Asignamos dos bloques, liberamos el segundo. Reasignando el primero para que crezca...\n");
            ptr1 = realloc(ptr1, 120); // Debería caber en el espacio de 50 + 50
            printf("Dirección del puntero después de realloc: %p (debería ser la misma que la original)\n", (void*)ptr1);
            free(ptr1);
            printf("Test 7 completado. Si el puntero no ha cambiado, el crecimiento en su lugar funciona.\n");
            break;
        case 8:
            // Test 8: Realloc para encogerse y dividir el bloque
            printf("--- Ejecutando Test 8: Realloc para encogerse ---\n");
            ptr1 = malloc(200);
            printf("Asignamos un bloque grande (200 bytes). Ahora lo reducimos a 50 bytes.\n");
            ptr2 = realloc(ptr1, 50);
            printf("Dirección del puntero después de realloc: %p (debería ser la misma que la original)\n", (void*)ptr2);
            free(ptr2);
            printf("Test 8 completado. show_alloc_mem debería mostrar un bloque libre resultante.\n");
            break;
        case 9:
            // Test 9: Realloc entre zonas
            printf("--- Ejecutando Test 9: Realloc entre zonas ---\n");
            // Caso 1: SMALL a LARGE
            ptr1 = malloc(100); // Bloque SMALL
            ft_memset(ptr1, 'A', 100);
            printf("Reasignando de SMALL a LARGE...\n");
            ptr2 = realloc(ptr1, 100000); // 100 KB
            printf("Puntero original (SMALL): %p | Nuevo puntero (LARGE): %p\n", (void*)ptr1, (void*)ptr2);
            free(ptr2);

            // Caso 2: LARGE a SMALL
            ptr1 = malloc(100000); // Bloque LARGE
            ft_memset(ptr1, 'B', 100000);
            printf("Reasignando de LARGE a SMALL...\n");
            ptr2 = realloc(ptr1, 100);
            printf("Puntero original (LARGE): %p | Nuevo puntero (SMALL): %p\n", (void*)ptr1, (void*)ptr2);
            free(ptr2);
            printf("Test 9 completado. Si los punteros cambian y no hay crashes, el realloc entre zonas funciona.\n");
            break;
        case 10:
            // Test 10: Llenado de memoria y liberaciones selectivas
            printf("--- Ejecutando Test 10: Llenado de memoria y liberaciones selectivas ---\n");
            char *chunks[500];
            printf("Llenando la memoria con bloques de 16 bytes...\n");
            for (i = 0; i < 500; i++) {
                chunks[i] = malloc(16);
            }
            show_alloc_mem();
            printf("Liberando bloques alternos...\n");
            for (i = 0; i < 500; i += 2) {
                free(chunks[i]);
            }
            printf("show_alloc_mem despues de liberar la mitad:\n");
            show_alloc_mem();
            printf("Liberando el resto de los bloques.\n");
            for (i = 1; i < 500; i += 2) {
                free(chunks[i]);
            }
            printf("show_alloc_mem despues de liberar todos los bloques:\n");
            show_alloc_mem();
            printf("Test 10 completado. Si la memoria se ha liberado por completo al final, la coalescencia funciona.\n");
            break;
        default:
            printf("Numero de test no valido.\n");
            break;
    }
    return 0;
}
