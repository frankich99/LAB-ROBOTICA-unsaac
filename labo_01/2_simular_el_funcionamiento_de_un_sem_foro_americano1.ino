/**
 * @institution Universidad Nacional de San Antonio Abad del Cusco (UNSAAC)
 * @program     Ingeniería Informática y de Sistemas
 * @area        Robótica
 * @file        Semaforo_Americano.ino
 * @brief       Secuencia semafórica vehicular (Verde -> Amarillo -> Rojo).
 * @target      Arduino Uno R3 (Tinkercad)
 */

// Asignación de pines
const uint8_t PIN_ROJO     = 12; // Fila 9
const uint8_t PIN_AMARILLO = 9;  // Fila 13
const uint8_t PIN_VERDE    = 7;  // Fila 17 (Corregida)

// Intervalos de tiempo (ms)
const uint16_t T_VERDE    = 5000;
const uint16_t T_AMARILLO = 2000;
const uint16_t T_ROJO     = 5000;

void setup() {
  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_AMARILLO, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);
}

void loop() {
  // 1. Estado Verde (Paso)
  digitalWrite(PIN_VERDE, HIGH);
  digitalWrite(PIN_AMARILLO, LOW);
  digitalWrite(PIN_ROJO, LOW);
  delay(T_VERDE);

  // 2. Estado Amarillo (Transición)
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_AMARILLO, HIGH);
  digitalWrite(PIN_ROJO, LOW);
  delay(T_AMARILLO);

  // 3. Estado Rojo (Alto)
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_AMARILLO, LOW);
  digitalWrite(PIN_ROJO, HIGH);
  delay(T_ROJO);
}