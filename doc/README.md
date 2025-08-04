### Guía de Abordaje para `ft_malloc`

#### **Fase 1: Configuración Inicial y Estructuras de Datos**

Antes de escribir la lógica de asignación, necesitas preparar el entorno y diseñar cómo vas a gestionar la memoria.

1.  [cite_start]**Configuración del `Makefile`**: El primer paso es asegurarte de que tu `Makefile` cumpla con los requisitos del subject[cite: 26, 27]. Esto incluye:
    * [cite_start]Asignar la variable de entorno `$HOSTTYPE` si no existe[cite: 28, 29, 30, 31, 32].
    * [cite_start]Compilar el proyecto para generar la biblioteca `libft_malloc_$HOSTTYPE.so`[cite: 25].
    * [cite_start]Crear un enlace simbólico `libft_malloc.so` que apunte a la biblioteca compilada[cite: 33, 34].
    * [cite_start]Asegurarte de que el `Makefile` compile también tu `libft` si la utilizas[cite: 36, 37].
2.  **Diseño de la Estructura de Memoria**: Piensa en la estructura de datos que usarás para gestionar los bloques de memoria. Necesitas una forma de saber si un bloque está libre u ocupado, su tamaño y, posiblemente, un puntero al siguiente bloque.
3.  [cite_start]**Identificación de los `syscalls`**: Recuerda que tu única herramienta para solicitar memoria al sistema es `mmap` y para devolverla es `munmap`[cite: 42, 43, 71]. [cite_start]Las zonas de memoria que solicites deben ser múltiplos del tamaño de página (`getpagesize` o `sysconf`)[cite: 75].

#### **Fase 2: Implementación de `malloc`**

Esta es la función principal. La clave es clasificar la petición de tamaño en una de las tres categorías y aplicar la lógica correcta.

1.  [cite_start]**Clasificación de la Petición**: Define los límites para las asignaciones "TINY" y "SMALL"[cite: 79, 81]. [cite_start]Cualquier petición que supere el límite de "SMALL" será "LARGE"[cite: 82].
2.  **Manejo de Asignaciones "LARGE"**: Cuando se solicita un bloque grande, la lógica es sencilla. Llama a `mmap` para asignar exactamente la cantidad de memoria que necesitas y retorna el puntero. [cite_start]No necesitas gestionar estas asignaciones en tus zonas internas[cite: 82].
3.  **Manejo de Asignaciones "TINY" y "SMALL"**:
    * **Buscar un hueco**: Primero, busca en tus zonas de memoria preexistentes un bloque libre que pueda acomodar la petición.
    * [cite_start]**Crear una nueva zona**: Si no hay espacio libre, necesitas pre-asignar una nueva zona con `mmap` que sea lo suficientemente grande para contener al menos 100 asignaciones del tamaño adecuado[cite: 74, 78]. Luego, asigna el bloque solicitado en esa nueva zona.
4.  [cite_start]**Alineación de la Memoria**: Asegúrate de que el puntero que devuelves al usuario esté correctamente alineado[cite: 99].

#### **Fase 3: Implementación de `free`**

Esta función debe ser robusta y eficiente.

1.  **Validación del Puntero**: El primer paso es validar el puntero que se recibe. [cite_start]Si es `NULL`, la función debe terminar sin hacer nada[cite: 69].
2.  **Liberación del Bloque**: Encuentra el bloque de memoria interno que corresponde al puntero del usuario y márcalo como libre en tu estructura de datos.
3.  **Consolidación de Bloques**: Para evitar la fragmentación, tu programa debería poder consolidar (unir) los bloques libres adyacentes.
4.  [cite_start]**Liberación de Zonas Completas**: Si todos los bloques de una zona (TINY o SMALL) están libres, puedes llamar a `munmap` para devolver toda la zona al sistema operativo, optimizando el uso de la memoria[cite: 73].

#### **Fase 4: Implementación de `realloc`**

Esta función combina la lógica de `malloc` y `free` con una copia de datos.

1.  [cite_start]**Manejo de Casos Especiales**: Si `ptr` es `NULL`, `realloc` debe comportarse como `malloc`[cite: 70].
2.  **Ajuste en el Lugar**: Intenta cambiar el tamaño del bloque en su posición actual. [cite_start]Si el nuevo tamaño cabe en el bloque original (con o sin consolidar con el siguiente bloque libre), ajusta el tamaño y retorna el mismo puntero[cite: 66].
3.  **Nueva Asignación**: Si el nuevo tamaño no cabe en el bloque actual, necesitas:
    * Llamar a tu `malloc` para obtener un nuevo bloque del tamaño solicitado.
    * [cite_start]Copiar la cantidad de datos que quepan del bloque antiguo al nuevo[cite: 67].
    * [cite_start]Llamar a tu `free` para liberar el bloque antiguo[cite: 67].
    * Retornar el puntero al nuevo bloque.

#### **Fase 5: Implementación de `show_alloc_mem`**

Esta función es clave para depurar tu programa.

1.  [cite_start]**Recorrer las Zonas**: Debes iterar a través de todas las zonas de memoria que has asignado (TINY, SMALL, LARGE)[cite: 88, 93, 95].
2.  [cite_start]**Detalles de la Salida**: Para cada zona, imprime el tipo de zona, su dirección inicial y los detalles de cada bloque asignado en ella[cite: 89, 90, 91, 92].
3.  [cite_start]**Total de Memoria**: Calcula y muestra el total de memoria asignada en todas las zonas[cite: 97].

Una vez que tengas la parte obligatoria funcionando a la perfección, podemos pasar a los bonus.

Adelante, puedes empezar por la primera fase. Cuando tengas dudas o creas que has completado una parte, pregúntame. Estoy aquí para guiarte.
