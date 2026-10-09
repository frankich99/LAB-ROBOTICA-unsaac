/**
 * @institution Universidad Nacional de San Antonio Abad del Cusco (UNSAAC)
 * @program     Ingeniería Informática y de Sistemas
 * @area        Robótica
 * @file        Semaforo_Autos_Peatones.ino
 * @brief       Control de tráfico vehicular y peatonal en ciclo continuo.
 * @target      Arduino Uno R3 (Tinkercad)
 */

// Mapeo de pines
const uint8_t CAR_VERDE  = 2;
const uint8_t CAR_AMAR   = 3;
const uint8_t CAR_ROJO   = 4;
const uint8_t PEAT_VERDE = 5;
const uint8_t PEAT_ROJO  = 6;

// Tiempos de operación (ms)
const uint16_t T_AUTOS_VERDE = 5000; // Paso vehicular
const uint16_t T_AUTOS_AMAR  = 2000; // Precaución vehicular
const uint16_t T_SEGURIDAD   = 1000; // Rojo total
const uint16_t T_PEAT_PASO   = 5000; // Cruce peatonal
const uint16_t T_PARPADEO    = 300;  // Alerta fin cruce

void setup() {
  pinMode(CAR_VERDE, OUTPUT);
  pinMode(CAR_AMAR, OUTPUT);
  pinMode(CAR_ROJO, OUTPUT);
  pinMode(PEAT_VERDE, OUTPUT);
  pinMode(PEAT_ROJO, OUTPUT);
}

void loop() {
  // 1. Tráfico vehicular abierto / Peatones detenidos
  digitalWrite(CAR_VERDE, HIGH);
  digitalWrite(CAR_AMAR, LOW);
  digitalWrite(CAR_ROJO, LOW);
  digitalWrite(PEAT_VERDE, LOW);
  digitalWrite(PEAT_ROJO, HIGH);
  delay(T_AUTOS_VERDE);

  // 2. Transición vehicular (Amarillo)
  digitalWrite(CAR_VERDE, LOW);
  digitalWrite(CAR_AMAR, HIGH);
  delay(T_AUTOS_AMAR);
  digitalWrite(CAR_AMAR, LOW);

  // 3. Margen de despeje (Rojo total)
  digitalWrite(CAR_ROJO, HIGH);
  delay(T_SEGURIDAD);

  // 4. Cruce peatonal habilitado
  digitalWrite(PEAT_ROJO, LOW);
  digitalWrite(PEAT_VERDE, HIGH);
  delay(T_PEAT_PASO);

  // 5. Advertencia fin de cruce (Parpadeo verde peatonal)
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(PEAT_VERDE, LOW);
    delay(T_PARPADEO);
    digitalWrite(PEAT_VERDE, HIGH);
    delay(T_PARPADEO);
  }
  digitalWrite(PEAT_VERDE, LOW);
  digitalWrite(PEAT_ROJO, HIGH);
  delay(T_SEGURIDAD);

  // 6. Fin de ciclo (Cede paso a nueva fase vehicular)
  digitalWrite(CAR_ROJO, LOW);
}