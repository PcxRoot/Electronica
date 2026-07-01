#include <LiquidCrystal.h>

// Inicializamos el objeto de LiQuidCrystal.
// Los parámetros que recibe el constructor son los pines
// de Arduino a los que está conectado en el siguiente orden
// (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12,11,10,9,8,7);

// Creamos el caracter personalizado con la forma de la calavera
uint8_t caravela[8] = {
  B01110,
  B11111,
  B10101,
  B11011,
  B01110,
  B01010,
  B01110,
  B00000
};

// Código que se ejecutará una sola vez al iniciar el programa
void setup() {
  // Encendemos la pantalla LCD con 20 columnas y 4 filas
  lcd.begin(20, 4);
  // Creamos el caracter personalizado de la calavera, 
  // y lo almacenamos en la dirección de memoria 0 (0x00)
  lcd.createChar(0, caravela);

  // Banner
  lcd.setCursor(10,0);
  lcd.write((uint8_t)0);
  lcd.setCursor(7, 1);
  lcd.print("Memory");
  lcd.setCursor(7, 2);
  lcd.print("Dumper");
  lcd.setCursor(10,3);
  lcd.write((uint8_t)0);
  delay(5000);
  lcd.clear();

  // Caracteres estáticos
  lcd.setCursor(0,0);
  lcd.write((uint8_t)0);
  lcd.setCursor(19,0);
  lcd.write((uint8_t)0);
  lcd.setCursor(0,3);
  lcd.write((uint8_t)0);
  lcd.setCursor(19,3);
  lcd.write((uint8_t)0);

  lcd.setCursor(3,1);
  lcd.print("Caracter ASCII:");
  lcd.setCursor(6, 2);
  lcd.print("0x");
}

void loop() {
  // Nos saltamos las primeras 32 direcciones de memoria,
  // en las que tan solo estan los caracteres que crea el usuario
  // y carcateres blank
  int i = 33;

  // Iteramos sobre la direcciones de memoria que si tienen caracteres
  while ( i <= 127 ) {
    lcd.setCursor(8,2);
    lcd.print(i, HEX);
    lcd.print(":[");
    lcd.write(i);
    lcd.print("]");
    delay(500);
    i++;
  }

  // Volvemos a saltarnos las direcciones de memoria con
  // caracteres blank
  while ( i >= 128 && i <= 160) {
    i++;
  }

  // Terminamos de iterar sobre los caracteres especiales
  // (como Katakana Japones)
  while ( i > 160 && i <= 225 ) {
    lcd.setCursor(8,2);
    lcd.print(i, HEX);
    lcd.print(":[");
    lcd.write(i);
    lcd.print("]");
    delay(500);
    i++;
  }
}