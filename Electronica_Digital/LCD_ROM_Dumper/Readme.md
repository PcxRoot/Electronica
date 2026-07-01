# Hardware Memory Dumper: Auditoría de sistemas Embebidos (*LCD 20x4*)

## Introducción

Este repositorio documenta el desarrollo y la implementación de un ***Memory Dumper*** diseñado para la arquitectura del controlador de pantalla LCD ***HD44780***.

Lejos de ser una simple herramienta de visualización, este proyecto sirve como un banco de pruebas para entender la **arquitectura interna de los sistemas embebidos**. A través de la ingeniería inversa, este repositorio explora cómo los dispositivos de hardware organizan y protegen (o exponen) sus datos en memoria, diferenciando entre los tres tipos de memoria crítica del chip:

- ***DDRAM (Display Data Ram):*** El área de trabajo donde residen los datos inyectados por el usuario.
- ***CGROM (Character Generator ROM):*** La capa de "solo lectura" donde el fabricante define la tipografía y los caracteres nativos.
- ***CGRAM (Character Generator RAM):*** El vector de inyección donde almacenamos *payloads* visuales personalizados (podemos crear hasta 8 caracteres personalizados).

## Por qué este proyecto?

Como estudiantye de ciberseguridad, entiendo que **el hardware es el cimiento de cualquier sistema**. Si un dispositivos no tiene protegida su memoria física o no entiende la separación entre zonas de datos y zonas de ejecución, se vuelve vulnerable.

Este proyecto permite:

1. ***Auditar la tabla de fuentes** de cualquier pantalla LCD compatible.
2. ***Extraer datos residentes** mediante técnicas de ***fuzzing*** lógico.
3. ***Comprender el "Memory Aliasing"***, un fallo de diseño común en hardware de bajo coste que permite el acceso no autorizado a registros internos.

## Objetivos del Repositorio

- ***Ingeniería Inversa:*** Documentar los mapas de memoria del *HD44780*.
- ***Fuzzing:*** Ejecutar volcados de memoria para identificar caracteres ocultos o funciones no documentadas.
- ***Inyección de Payload:*** Demostrar cómo inyectar caracteres personalizados (ej. iconos de seguridad) sobrescribiendo la memoria RAM volátil.

---

>"La seguridad empieza en el silicio. si puedes leer su memoria, puedes entender su lógica."

---

>[Important]
>*Disclaimer: Este proyecto es exclusivamente con fines educativos y de auditoría de hardware. Utilizar estos métodos en dispositivos ajenos sin autorización es ilegal.*

---


