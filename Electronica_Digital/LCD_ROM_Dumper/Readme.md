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

>[!Important]
>*Disclaimer: Este proyecto es exclusivamente con fines educativos y de auditoría de hardware. Utilizar estos métodos en dispositivos ajenos sin autorización es ilegal.*

---

## Memorias

### DDRAM (Display Data RAM)

En el controlador *HD44780*, la ***DDRAM*** es el ***área de almacenamiento de lectura y escritura (volatil) que se encarga exclusivamente de guardar los códigos de los caracteres que se están mostrando en pantalla.

#### Análisis Técnico (Ingeniería Inversa)

##### 1. Capacidad y Organización

- ***Capacidad total:*** 80 bytes.
- ***Relación visual:*** Aunque nuestra pantalla sea de *16x2* o *20x4*, el chip internamente tiene espacio para 80 caracteres (1 byte/caracter).
- ***El "Scroll" físico:*** Si nuestra pantalla es de *16x2* (32 espacios visibles), los 48 bytes restantes siguen ahí, en la DDRAM, aunque no se vean. Son como **espacios de memroia ocultos** que podemos usar para desplazar texto (hacer scroll) sin tener que volver a enviar los datos desde el ***Arduino***.

>[!Note]
>Imagina que usamos el comando `lcd.print("Estoy aprendiendo electronica")`, veremos que tiene 29 caracteres, por lo que, tendriamos que usar las dos líenas de una pantalla LCD 16x2 para poder mostrar todo el mensaje.
>
>Para hacre el efecto de *Scroll*, podemos hacer uso de `lcd.scrollDisplayLeft()` el cual es un método que nos permite cargar todos el mensaje anterior en la memoria DDRAM e ir "*scrolleando*" un caracter a la vez para mostrar todo el mensaje sin tener que mostrar una parte en la primera linea y el resto en la segunda.
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

No obstante, esto puede llegara  dar problemas, ya que lo que hace el métdo `lcd.scrollDisplayLeft()` es ***desplazar todo el banco de memoria***, pero no afecta a direcciones de memoria específica. Esto quiere decir que podemos usar esta técnica para mostrar un mensaje de hasta 80bytes / 2 líneas = 40 caracteres en una linea. Si nos pasamos de este límite, al chip se le acabará los espacios de memoria para la primera fila y tendrá que hacer un salto para poder mostrar todo el mensaje.

Si queremos poder mostrar un mensaje de cualquier longitud (incluso mayor a 80) podemos usar un truco consistente en partir el mensaje en secciones de 16 caracteres e ir restando el primero y sumando uno nuevo cada iteración. De esta forma damos el efecto de *Scroll* aunque eralmente no estamos moviendo la pantalla.

```C++
String mensaje = "Este es un mensaje que es tan largo que no deberia de caber "
"en la memoria DDRAM del chip de la pantalla LCD";
for ( int i = 0; i <= (mensaje.length() - 16); i++ ) {
  lcd.setCursor(0, 0);
  lcd.print(mensaje.substring(i, i + 16));
  delay(300);
}
```





























