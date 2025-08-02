# ft_ls ✅ Proyecto Terminado y Perfecto

Este proyecto es una reimplementación del comando `ls` de UNIX en lenguaje C. Ha sido desarrollado en el marco del cursus 42, con el objetivo de comprender en profundidad cómo interactuar con el sistema de archivos a bajo nivel.

---

## 🧠 ¿De qué trata?

`ft_ls` es un clon del comando `ls`, ampliamente utilizado para listar archivos y directorios en sistemas UNIX. Este proyecto permite profundizar en el manejo de directorios, archivos especiales, permisos, fechas, tamaños y estructuras del sistema operativo a través de llamadas al sistema.

---

## 📌 Objetivos del proyecto

- Aprender a manipular estructuras de datos del sistema operativo como `struct dirent` o `struct stat`.
- Comprender cómo funcionan internamente comandos fundamentales del shell.
- Reforzar el uso de funciones de la libc que no se habían utilizado anteriormente.
- Consolidar buenas prácticas en el diseño de código modular, reutilizable y robusto.

---

## ⚙️ Comportamiento esperado

Tu implementación **debe comportarse como el comando `ls`** del sistema operativo, al menos en lo relativo a las siguientes opciones:

- `-l`: Modo largo con información detallada.
- `-R`: Recursividad en los directorios.
- `-a`: Mostrar archivos ocultos.
- `-r`: Orden inverso.
- `-t`: Ordenar por fecha de modificación.

> ⚠️ No es necesario manejar ACLs ni atributos extendidos.  
> ✅ No se requiere formato por columnas si no se usa la opción `-l`.

---

## 💡 Bonus (implementados opcionalmente)

Si el proyecto obligatorio ha sido completado perfectamente, se pueden añadir funcionalidades adicionales como:

- Soporte para ACL y atributos extendidos.
- Visualización en columnas sin `-l`.
- Nuevas opciones (`-u`, `-f`, `-g`, `-d`, etc.).
- Vista con colores (como `ls -G`).
- Optimización del rendimiento en listados muy grandes (`ls -lR` sobre muchos ficheros).

---

## 🛠️ Reglas técnicas

- El proyecto debe compilarse con un `Makefile` completo.
- El ejecutable debe llamarse `ft_ls`.
- Se permite y recomienda el uso de una librería propia (`libft`).
- El código debe estar completamente libre de errores de memoria (sin leaks).
- Se deben manejar los errores con elegancia, imitando el comportamiento de `ls`.

---

## 📂 Compilación

```bash
make
./ft_ls [opciones] [ficheros o directorios]
````

---

## ✍️ Evaluación

La corrección será **100% humana**, y solo se evaluará el código dentro del repositorio. La parte bonus solo será tenida en cuenta si **la parte obligatoria está completamente perfecta**.

---

Desarrollado como parte del cursus 42, este proyecto demuestra una comprensión profunda de los mecanismos internos del sistema UNIX. ✅

```

¿Quieres que incluya también instrucciones para probarlo o subirlo a GitHub directamente con ese `README.md` como primer commit?
```
