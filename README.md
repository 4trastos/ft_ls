# malloc ✅ Proyecto Terminado y Perfecto

### Un proyecto de gestión de memoria dinámica en C

## 📝 Resumen del Proyecto

Este proyecto consiste en la reimplementación de una biblioteca de gestión de memoria dinámica, replicando las funcionalidades de las funciones estándar `malloc`, `free` y `realloc` de la `libc`. El objetivo principal es comprender cómo funciona la gestión de memoria a bajo nivel, utilizando `mmap` y `munmap` para asignar y liberar zonas de memoria del sistema operativo.

## 📋 Funcionalidades Obligatorias (Mandatory Part)

La biblioteca debe proporcionar las siguientes funciones con las mismas firmas que sus contrapartes de la `libc`:

  * `void *malloc(size_t size)`: Asigna un bloque de `size` bytes de memoria y devuelve un puntero al inicio de ese bloque. En caso de error, debe devolver `NULL`.
  * `void free(void *ptr)`: Desasigna el bloque de memoria apuntado por `ptr`. Si `ptr` es `NULL`, la función no realiza ninguna operación.
  * `void *realloc(void *ptr, size_t size)`: Intenta cambiar el tamaño del bloque de memoria apuntado por `ptr` al nuevo `size`. Si no hay suficiente espacio para agrandar la asignación, crea una nueva, copia los datos, libera la antigua y devuelve un puntero a la nueva. En caso de error, devuelve `NULL`.

### Mecanismo de Asignación

Para optimizar el rendimiento y minimizar las llamadas al sistema, el gestor de memoria debe clasificar las asignaciones en tres categorías:

  * **TINY**: Asignaciones pequeñas, de 1 a `n` bytes. Se agrupan en zonas de tamaño fijo (por ejemplo, `N` bytes).
  * **SMALL**: Asignaciones medianas, de `n+1` a `m` bytes. Se agrupan en zonas de tamaño fijo (por ejemplo, `M` bytes).
  * **LARGE**: Asignaciones grandes, de `m+1` bytes en adelante. Cada una se gestiona con una llamada individual a `mmap`.

Los tamaños de las zonas (`N` y `M`) deben ser múltiplos del tamaño de página del sistema, que se puede obtener con `getpagesize()` (en macOS) o `sysconf(_SC_PAGESIZE)` (en Linux). Cada zona debe ser capaz de contener al menos 100 asignaciones.

Además, la memoria devuelta por `malloc` debe estar alineada.

### Función de Visualización

Se requiere una función para depuración que muestre el estado de las zonas de memoria asignadas:

  * `void show_alloc_mem(void)`

La salida debe estar formateada de manera clara, mostrando las diferentes zonas (TINY, SMALL, LARGE) y el tamaño total de la memoria asignada.

## ⚙️ Uso de la Biblioteca

### Compilación

El proyecto debe compilarse con un `Makefile` que incluya las reglas habituales. La biblioteca se debe nombrar `libft_malloc_$HOSTTYPE.so`, donde `HOSTTYPE` es una variable de entorno. El `Makefile` debe gestionar esta variable y crear un enlace simbólico `libft_malloc.so` que apunte a la biblioteca.

```makefile
ifeq ($(HOSTTYPE),)
HOSTTYPE:= $(shell uname -m) $(shell uname -s)
endif
```

### Funciones Autorizadas

Para la parte obligatoria, solo se permite el uso de las siguientes funciones y llamadas al sistema:

  * `mmap(2)`, `munmap(2)`
  * `getpagesize` o `sysconf(_SC_PAGESIZE)`
  * `getrlimit(2)`
  * Funciones de tu propia `libft` (como `write(2)`).
  * Se permite el uso de una variable global para la gestión de la memoria y otra para el `thread-safe`.

## ✨ Parte Opcional (Bonus)

La parte opcional solo será evaluada si la parte obligatoria es perfecta.

  * **Thread-safe**: Implementar la gestión de memoria en programas multihilo para garantizar que sea segura.
  * **Variables de entorno de depuración**: Replicar o crear variables de entorno que permitan depurar la biblioteca `malloc`, como las del sistema.
  * **Función de depuración avanzada**: Crear una función `show_alloc_mem_ex()` que muestre más detalles sobre las asignaciones (por ejemplo, un historial o un `hexa dump`).
  * **Desfragmentación**: Implementar un mecanismo para desfragmentar la memoria liberada.
