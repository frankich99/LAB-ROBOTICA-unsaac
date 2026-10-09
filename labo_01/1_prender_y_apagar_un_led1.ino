/**
 * @institution Universidad Nacional de San Antonio Abad del Cusco (UNSAAC)
 * @program     Ingeniería Informática y de Sistemas
 * @area        Robótica
 * @file        Blink_LED.ino
 * @brief       Control de parpadeo de un LED por salida digital.
 * @target      Arduino Uno R3 (Tinkercad)
 */

// Constantes
const uint8_t PIN_LED = 13;          // Pin salida LED
const uint16_t TIEMPO_ESPERA = 1000; // Periodo (ms)

// Configuración de puertos
void setup() {
  pinMode(PIN_LED, OUTPUT); // Pin 13 como salida
}

// Ciclo de control
void loop() {
  digitalWrite(PIN_LED, HIGH); // LED ON (5V)
  delay(TIEMPO_ESPERA);        // Espera
  digitalWrite(PIN_LED, LOW);  // LED OFF (0V)
  delay(TIEMPO_ESPERA);        // Espera
}