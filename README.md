# malloc ✅ Proyecto Terminado y Perfecto

### Un proyecto de gestión de memoria dinámica en C

## 📝 Resumen del Proyecto

[cite\_start]Este proyecto consiste en la reimplementación de una biblioteca de gestión de memoria dinámica, replicando las funcionalidades de las funciones estándar `malloc`, `free` y `realloc` de la `libc`[cite: 3, 56]. [cite\_start]El objetivo principal es comprender cómo funciona la gestión de memoria a bajo nivel, utilizando `mmap` y `munmap` para asignar y liberar zonas de memoria del sistema operativo[cite: 71].

## 📋 Funcionalidades Obligatorias (Mandatory Part)

[cite\_start]La biblioteca debe proporcionar las siguientes funciones con las mismas firmas que sus contrapartes de la `libc`[cite: 57, 58, 59, 60, 61, 62, 63, 64]:

  * [cite\_start]`void *malloc(size_t size)`: Asigna un bloque de `size` bytes de memoria y devuelve un puntero al inicio de ese bloque[cite: 65]. [cite\_start]En caso de error, debe devolver `NULL`[cite: 70].
  * [cite\_start]`void free(void *ptr)`: Desasigna el bloque de memoria apuntado por `ptr`[cite: 68]. [cite\_start]Si `ptr` es `NULL`, la función no realiza ninguna operación[cite: 69].
  * [cite\_start]`void *realloc(void *ptr, size_t size)`: Intenta cambiar el tamaño del bloque de memoria apuntado por `ptr` al nuevo `size`[cite: 66]. [cite\_start]Si no hay suficiente espacio para agrandar la asignación, crea una nueva, copia los datos, libera la antigua y devuelve un puntero a la nueva[cite: 67]. [cite\_start]En caso de error, devuelve `NULL`[cite: 70].

### Mecanismo de Asignación

[cite\_start]Para optimizar el rendimiento y minimizar las llamadas al sistema, el gestor de memoria debe clasificar las asignaciones en tres categorías[cite: 73, 74, 79, 81, 82]:

  * [cite\_start]**TINY**: Asignaciones pequeñas, de 1 a `n` bytes[cite: 79]. Se agrupan en zonas de tamaño fijo (por ejemplo, `N` bytes).
  * [cite\_start]**SMALL**: Asignaciones medianas, de `n+1` a `m` bytes[cite: 81]. Se agrupan en zonas de tamaño fijo (por ejemplo, `M` bytes).
  * [cite\_start]**LARGE**: Asignaciones grandes, de `m+1` bytes en adelante[cite: 82]. Cada una se gestiona con una llamada individual a `mmap`.

[cite\_start]Los tamaños de las zonas (`N` y `M`) deben ser múltiplos del tamaño de página del sistema, que se puede obtener con `getpagesize()` (en macOS) o `sysconf(_SC_PAGESIZE)` (en Linux)[cite: 75]. [cite\_start]Cada zona debe ser capaz de contener al menos 100 asignaciones[cite: 78].

[cite\_start]Además, la memoria devuelta por `malloc` debe estar alineada[cite: 99].

### Función de Visualización

[cite\_start]Se requiere una función para depuración que muestre el estado de las zonas de memoria asignadas[cite: 84]:

  * `void show_alloc_mem(void)`

[cite\_start]La salida debe estar formateada de manera clara, mostrando las diferentes zonas (TINY, SMALL, LARGE) y el tamaño total de la memoria asignada[cite: 87, 88, 93, 95, 97].

## ⚙️ Uso de la Biblioteca

### Compilación

[cite\_start]El proyecto debe compilarse con un `Makefile` que incluya las reglas habituales[cite: 26]. [cite\_start]La biblioteca se debe nombrar `libft_malloc_$HOSTTYPE.so`, donde `HOSTTYPE` es una variable de entorno[cite: 25]. [cite\_start]El `Makefile` debe gestionar esta variable y crear un enlace simbólico `libft_malloc.so` que apunte a la biblioteca[cite: 33].

```makefile
ifeq ($(HOSTTYPE),)
HOSTTYPE:= $(shell uname -m) $(shell uname -s)
endif
```

### Funciones Autorizadas

Para la parte obligatoria, solo se permite el uso de las siguientes funciones y llamadas al sistema:

  * [cite\_start]`mmap(2)`, `munmap(2)` [cite: 42, 43]
  * [cite\_start]`getpagesize` o `sysconf(_SC_PAGESIZE)` [cite: 44]
  * [cite\_start]`getrlimit(2)` [cite: 45]
  * [cite\_start]Funciones de tu propia `libft` (como `write(2)`)[cite: 48].
  * [cite\_start]Se permite el uso de una variable global para la gestión de la memoria y otra para el `thread-safe`[cite: 38].

## ✨ Parte Opcional (Bonus)

[cite\_start]La parte opcional solo será evaluada si la parte obligatoria es perfecta[cite: 110, 111, 112].

  * [cite\_start]**Thread-safe**: Implementar la gestión de memoria en programas multihilo para garantizar que sea segura[cite: 104].
  * [cite\_start]**Variables de entorno de depuración**: Replicar o crear variables de entorno que permitan depurar la biblioteca `malloc`, como las del sistema[cite: 106].
  * [cite\_start]**Función de depuración avanzada**: Crear una función `show_alloc_mem_ex()` que muestre más detalles sobre las asignaciones (por ejemplo, un historial o un `hexa dump`)[cite: 107].
  * [cite\_start]**Desfragmentación**: Implementar un mecanismo para desfragmentar la memoria liberada[cite: 108].
