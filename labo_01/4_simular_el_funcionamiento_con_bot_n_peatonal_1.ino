/**
 * @institution Universidad Nacional de San Antonio Abad del Cusco (UNSAAC)
 * @program     Ingeniería Informática y de Sistemas
 * @area        Robótica
 * @file        Semaforo_Demanda_Optimo.ino
 * @brief       Semáforo por demanda con registro de petición y tiempo mínimo garantizado.
 * @target      Arduino Uno R3 (Tinkercad)
 */

// Asignación de pines
#define REDCARS           13
#define YELLOWCARS        12
#define GREENCARS         11
#define GREENPEDESTRIANS  10
#define REDPEDESTRIANS     9
#define BUTTON             6
#define SIGNAL             3

// Parámetros de tiempo (ms)
const unsigned long T_VERDE_MIN   = 6000; // Tiempo mínimo de fluidez vehicular (6 s)
const unsigned long T_AMARILLO    = 2500; // Transición y frenado vehicular (2.5 s)
const unsigned long T_SEGURIDAD   = 1000; // Rojo total entre cambios (1 s)
const unsigned long T_PEATON_PASO = 6000; // Cruce peatonal (6 s)
const unsigned long T_PARPADEO    = 350;  // Frecuencia destello verde peatón

// Variables de control
unsigned long tiempoInicioVerde = 0;
bool peticionCruce = false;

void setup() {
  pinMode(REDCARS, OUTPUT);
  pinMode(YELLOWCARS, OUTPUT);
  pinMode(GREENCARS, OUTPUT);
  pinMode(REDPEDESTRIANS, OUTPUT);
  pinMode(GREENPEDESTRIANS, OUTPUT);
  pinMode(SIGNAL, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP);

  // Estado inicial: Tráfico vehicular habilitado
  digitalWrite(GREENCARS, HIGH);
  digitalWrite(YELLOWCARS, LOW);
  digitalWrite(REDCARS, LOW);
  digitalWrite(REDPEDESTRIANS, HIGH);
  digitalWrite(GREENPEDESTRIANS, LOW);
  digitalWrite(SIGNAL, LOW);

  tiempoInicioVerde = millis();
}

void loop() {
  // 1. Escaneo permanente del pulsador (almacena la petición al primer clic)
  if (digitalRead(BUTTON) == LOW) {
    peticionCruce = true;
  }

  // 2. Si existe petición, evalúa si ya se cumplió la ventana mínima vehicular
  if (peticionCruce && (millis() - tiempoInicioVerde >= T_VERDE_MIN)) {
    ejecutarFasePeatonal();
    peticionCruce = false;           // Limpia la bandera de solicitud
    tiempoInicioVerde = millis();    // Reinicia el contador para el nuevo verde de autos
  }
}

// Secuencia controlada de cruce
void ejecutarFasePeatonal() {
  // Fase 1: Aviso amarillo vehicular y advertencia sonora
  digitalWrite(GREENCARS, LOW);
  digitalWrite(YELLOWCARS, HIGH);
  digitalWrite(SIGNAL, HIGH);
  delay(T_AMARILLO);
  digitalWrite(SIGNAL, LOW);
  digitalWrite(YELLOWCARS, LOW);

  // Fase 2: Intervalo de seguridad (Todos en rojo)
  digitalWrite(REDCARS, HIGH);
  delay(T_SEGURIDAD);

  // Fase 3: Cruce peatonal activo
  digitalWrite(REDPEDESTRIANS, LOW);
  digitalWrite(GREENPEDESTRIANS, HIGH);
  delay(T_PEATON_PASO);

  // Fase 4: Destello verde peatonal (aviso fin de cruce)
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(GREENPEDESTRIANS, LOW);
    delay(T_PARPADEO);
    digitalWrite(GREENPEDESTRIANS, HIGH);
    delay(T_PARPADEO);
  }
  digitalWrite(GREENPEDESTRIANS, LOW);
  digitalWrite(REDPEDESTRIANS, HIGH);
  delay(T_SEGURIDAD);

  // Fase 5: Restauración de flujo vehicular
  digitalWrite(REDCARS, LOW);
  digitalWrite(GREENCARS, HIGH);
}