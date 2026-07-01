# Hardware Memory Dumper: Auditoría de sistemas Embebidos (*LCD 20x4*)

## Introducción

Este repositorio documenta el desarrollo y la implementación de un ***Memory Dumper*** diseñado para la arquitectura del controlador de pantalla LCD ***HD44780***.

Lejos de ser una simple herramienta de visualización, este proyecto sirve como un banco de pruebas para entender la **arquitectura interna de los sistemas embebidos**. A través de la ingeniería inversa, este repositorio explora cómo los dispositivos de hardware organizan y protegen (o exponen) sus datos en memoria, diferenciando entre los tres tipos de memoria crítica del chip:

- ***DDRAM (Display Data Ram):*** El área de trabajo donde residen los datos inyectados por el usuario.
- ***CGROM (Character Generator ROM):*** La capa de "solo lectura" donde el fabricante define la tipografía y los caracteres nativos.
- ***CGRAM (Character Generator RAM):*** El vector de inyección donde almacenamos *payloads* visuales personalizados (podemos crear hasta 8 caracteres personalizados).

## Por qué este proyecto?

Como estudiante de ciberseguridad, entiendo que **el hardware es el cimiento de cualquier sistema**. Si un dispositivo no tiene protegida su memoria física o no entiende la separación entre zonas de datos y zonas de ejecución, se vuelve vulnerable.

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

>[!Important]
>*Disclaimer: Este proyecto es exclusivamente con fines educativos y de auditoría de hardware. Utilizar estos métodos en dispositivos ajenos sin autorización es ilegal.*

---

## Memorias

### DDRAM (Display Data RAM)

En el controlador *HD44780*, la ***DDRAM*** es el ***área de almacenamiento de lectura y escritura (volátil) que se encarga exclusivamente de guardar los códigos de los caracteres que se están mostrando en pantalla.

#### Análisis Técnico (Ingeniería Inversa)

##### 1. Capacidad y Organización

- ***Capacidad total:*** 80 bytes.
- ***Relación visual:*** Aunque nuestra pantalla sea de *16x2* o *20x4*, el chip internamente tiene espacio para 80 caracteres (1 byte/carácter).
- ***El "Scroll" físico:*** Si nuestra pantalla es de *16x2* (32 espacios visibles), los 48 bytes restantes siguen ahí, en la DDRAM, aunque no se vean. Son como **espacios de memoria ocultos** que podemos usar para desplazar texto (hacer scroll) sin tener que volver a enviar los datos desde el ***Arduino***.

>[!Note]
>Imagina que usamos el comando `lcd.print("Estoy aprendiendo electronica")`, veremos que tiene 29 caracteres, por lo que, tendriamos que usar las dos líneas de una pantalla LCD 16x2 para poder mostrar todo el mensaje.
>
>Para hacer el efecto de *Scroll*, podemos hacer uso de `lcd.scrollDisplayLeft()` el cual es un método que nos permite cargar todo el mensaje anterior en la memoria DDRAM e ir "*scrolleando*" un caracter a la vez para mostrar todo el mensaje sin tener que mostrar una parte en la primera línea y el resto en la segunda.
>
>Ejemplo:
>```C++
>String mensaje = "Estoy aprendido electronica";
>lcd.setCursor(0,0);
>lcd.print(mensaje);
>delay(500);
>for ( int i = 0; i <= (mensaje.length() - 16); i++ ) {
>  lcd.scrollDisplayLeft();
>  delay(300);
>}
>```

No obstante, esto puede llegar a dar problemas, ya que lo que hace el método `lcd.scrollDisplayLeft()` es ***desplazar todo el banco de memoria***, pero no afecta a direcciones de memoria específicas. Esto quiere decir que podemos usar esta técnica para mostrar un mensaje de hasta 80bytes / 2 líneas = 40 caracteres en una línea. Si nos pasamos de este límite, al chip se le acabarán los espacios de memoria para la primera fila y tendrá que hacer un salto para poder mostrar todo el mensaje.

Si queremos poder mostrar un mensaje de cualquier longitud (incluso mayor a 80) podemos usar un truco consistente en partir el mensaje en secciones de 16 caracteres e ir restando el primero y sumando uno nuevo cada iteración. De esta forma damos el efecto de *Scroll* aunque realmente no estamos moviendo la pantalla.

```C++
String mensaje = "Este es un mensaje que es tan largo que no deberia de caber "
"en la memoria DDRAM del chip de la pantalla LCD";
for ( int i = 0; i <= (mensaje.length() - 16); i++ ) {
  lcd.setCursor(0, 0);
  lcd.print(mensaje.substring(i, i + 16));
  delay(300);
}
```

##### 2. Mapa de Direcciones (Memory Mapping)

Es importante tener en cuenta cómo se distribuyen las direcciones de la memoria:

- ***Línea 1:*** Comienza en la dirección `0x00` y llega hasta la `0x27` (en decimal del 0 al 39)
- ***Línea 2:*** Comienza en la dirección `0x40` y llega hasta la `0x67` (en decimal del 64 al 103)
- ***En total:*** `80 bytes`

### Por qué es crítica para un Auditor de Hardware?

La memoria DDRAM es el ***Vector de Entrada*** principal:

1. ***Inyección:*** Cuando hacemos `lcd.print("HOLA");`, estamos realizando una operación de escritura directa en la ***DDRAM***.
2. ***Persistencia:*** La DDRAM es volatil. Si cortamos la alimentación del Arduino, los datos de la DDRAM desaparecerán (a diferencia de la ROM que es permanente). Esto es vital para pruebas de ***Cold Boot attacks*** (ver si el hardware retiene información después de un reinicio).
3. ***Vulnerabilidad de Sobreescritura:*** Si no controlamos los punteros de la DDRAM, podemos sobrescribir datos que se estaban visualizando en la pantalla, un fenómeno que en software llamamos *Corrupción de Memoria o Buffer Overflow de interfaz*. 

### Resumen

***DDRAM (Display Data RAM)***
- ***Tipo:*** SRAM *(Memoria Estática de Acceso Aleatorio)*
- ***Función:*** Almacena el código ASCII de los caracteres a mostrar.
- ***Accesibilidad:*** Lectura y Escritura completa desde el bus del controlador.
- ***Nota de Hacking:*** Cualquier dato que envíes a la pantalla reside aquí. Es el lugar perfecto para buscar leaks de información o realizar inyecciones de texto dinámico.

























