// ==============================================================================
// UNIVERSIDAD NACIONAL DE SAN ANTONIO ABAD DEL CUSCO (UNSAAC)
// FACULTAD DE INGENIERÍA ELÉCTRICA, ELECTRÓNICA, INFORMÁTICA Y MECÁNICA
// ESCUELA PROFESIONAL DE INGENIERÍA INFORMÁTICA Y DE SISTEMAS
// 
// ASIGNATURA : Robótica
// PRÁCTICA   : Laboratorio N.° 03 - Medición de Distancia con ESP32 y HC-SR04
// AUTOR      : Noe Franklin Choquenaira Quispe (Cód. 133962)
// DOCENTE    : Jose Mauro Pillco Quispe
// SEMESTRE   : 2026-II
// ==============================================================================
// ARCHIVO: esp32_hcsr04_basic.ino
// DESCRIPCIÓN:
//   Implementación directa y fiel al Listing 1 presentado en el informe de
//   laboratorio. Utiliza pulseIn() para medir el tiempo de vuelo del pulso
//   ultrasónico emitido por el transductor piezoeléctrico del HC-SR04, calcula
//   la distancia física en centímetros y aplica umbrales de zona ciega y rango
//   práctico de operación.
// ==============================================================================

#define TRIG_PIN 5
#define ECHO_PIN 18

// Parámetros temporales y umbrales de medición
const unsigned long INTERVALO = 100;    // Intervalo de muestreo en milisegundos (ms)
const unsigned long TIMEOUT   = 30000;  // Tiempo máximo de espera del eco en microsegundos (µs) -> ~5.1 metros máx
const float DISTANCIA_CERO    = 3.0;    // Umbral de zona ciega inferior (cm). Lecturas <= 3cm se consideran 0.00 cm
const float DISTANCIA_MAX     = 100.0;  // Rango práctico máximo de medición (cm) para el entorno del laboratorio

/**
 * @brief Genera el pulso ultrasónico y calcula la distancia en centímetros.
 * @return Distancia calculada en cm, o -1.0 si no se detectó eco dentro del TIMEOUT.
 */
float medirDistancia()
{
  // 1. Asegurar nivel bajo inicial para estabilizar la línea de disparo
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 2. Pulso de disparo de 10 microsegundos requerido por el HC-SR04
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 3. Medición del ancho de pulso en alto generado por el pin ECHO
  // pulseIn espera a que el pin pase a HIGH, empieza a contar, y se detiene cuando vuelve a LOW.
  unsigned long tiempo = pulseIn(ECHO_PIN, HIGH, TIMEOUT);

  // Si se superó el TIMEOUT sin recibir respuesta, pulseIn devuelve 0
  if (tiempo == 0)
  {
    return -1.0;
  }

  // 4. Conversión de tiempo de vuelo a distancia:
  //    d = (tiempo_ida_y_vuelta * velocidad_sonido) / 2
  //    v_sonido ≈ 343 m/s = 0.0343 cm/µs
  float d = (tiempo * 0.0343) / 2.0;

  return d;
}

void setup()
{
  // Inicialización de la comunicación serial a 115200 baudios
  Serial.begin(115200);

  // Configuración de los pines GPIO del ESP32
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Estado inicial del pin de disparo
  digitalWrite(TRIG_PIN, LOW);

  // Tiempo de estabilización de la alimentación y del sensor
  delay(500);

  Serial.println("==================================================");
  Serial.println("  UNSAAC - Laboratorio 03: ESP32 + HC-SR04        ");
  Serial.println("  Medición de distancia por ultrasonido           ");
  Serial.println("==================================================");
  Serial.println("HC-SR04 listo");
}

void loop()
{
  float distancia = medirDistancia();

  // Tratamiento de condiciones especiales y reporte
  if (distancia < 0)
  {
    // Ausencia de eco válido (tiempo agotado / fuera de alcance)
    Serial.println("SIN OBJETO");
  }
  else if (distancia <= DISTANCIA_CERO)
  {
    // Objeto pegado a los transductores (zona ciega <= 3 cm)
    Serial.println("Distancia: 0.00 cm");
  }
  else if (distancia <= DISTANCIA_MAX)
  {
    // Medición válida dentro del rango experimental útil (3.0 cm a 100.0 cm)
    Serial.print("Distancia: ");
    Serial.print(distancia, 2);
    Serial.println(" cm");
  }
  else
  {
    // Distancia superior al rango práctico establecido
    Serial.println("SIN OBJETO");
  }

  // Intervalo entre mediciones sucesivas
  delay(INTERVALO);
}
