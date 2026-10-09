// ==============================================================================
// UNIVERSIDAD NACIONAL DE SAN ANTONIO ABAD DEL CUSCO (UNSAAC)
// FACULTAD DE INGENIERÍA ELÉCTRICA, ELECTRÓNICA, INFORMÁTICA Y MECÁNICA
// ESCUELA PROFESIONAL DE INGENIERÍA INFORMÁTICA Y DE SISTEMAS
//
// ASIGNATURA : Robótica
// PRÁCTICA   : Laboratorio N.° 03 - Medición de Distancia con ESP32 y HC-SR04
// AUTOR      : Noe Franklin Choquenaira Quispe (Cód. 133962)
// DOCENTE    : Jose Mauro Pillco Quispe
// ==============================================================================
// ARCHIVO: esp32_hcsr04_modular.ino
// DESCRIPCIÓN:
//   Versión modular, orientada a objetos y con arquitectura profesional.
//   - Encapsula el sensor en la clase HCSR04Sensor.
//   - Utiliza temporización no bloqueante basada en millis() para evitar delays.
//   - Incorpora compensación térmica de la velocidad del sonido.
//   - Aplica filtro de mediana para eliminar espurias y rebotes espurios.
//   - Soporta tres formatos de salida serial: Human-Readable, CSV y JSON.
// ==============================================================================

#include "config.h"
#include "HCSR04.h"

// Instanciación del sensor ultrasónico con parámetros de configuración
HCSR04Sensor sensor(
    PIN_TRIG, 
    PIN_ECHO, 
    ECHO_TIMEOUT_US, 
    DISTANCIA_MIN_CM, 
    DISTANCIA_MAX_CM, 
    calcularVelocidadSonido(TEMPERATURA_AMBIENTE_C)
);

// Variable para control de tiempo no bloqueante
unsigned long tiempoAnteriorMs = 0;

void imprimirCabeceraInicio()
{
  Serial.println();
  Serial.println("================================================================");
  Serial.println("   UNSAAC - LAB 03: SISTEMA DE MEDICION ULTRASONICA ESP32        ");
  Serial.println("   Sensor HC-SR04 | Arquitectura Modular C++ y No Bloqueante    ");
  Serial.println("================================================================");
  Serial.print(" - Pin TRIG (Disparo)        : GPIO "); Serial.println(PIN_TRIG);
  Serial.print(" - Pin ECHO (Recepción)       : GPIO "); Serial.println(PIN_ECHO);
  Serial.print(" - Temperatura Ambiente      : "); Serial.print(TEMPERATURA_AMBIENTE_C, 1); Serial.println(" °C");
  Serial.print(" - Velocidad del Sonido      : "); Serial.print(calcularVelocidadSonido(TEMPERATURA_AMBIENTE_C) * 10000.0f, 2); Serial.println(" m/s");
  Serial.print(" - Rango Operativo Útil      : ["); Serial.print(DISTANCIA_MIN_CM, 1); Serial.print(" cm - "); Serial.print(DISTANCIA_MAX_CM, 1); Serial.println(" cm]");
  Serial.print(" - Muestreo No Bloqueante    : cada "); Serial.print(SAMPLING_INTERVAL_MS); Serial.println(" ms");
  Serial.println("================================================================");
  Serial.println("HC-SR04 listo para mediciones");
  Serial.println();
}

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial && millis() < 3000) {
    // Espera activa defensiva para consolas CDC/USB nativas si aplica
  }

  sensor.begin();
  imprimirCabeceraInicio();

  tiempoAnteriorMs = millis();
}

void loop()
{
  unsigned long tiempoActualMs = millis();

  // Control no bloqueante del ciclo de muestreo
  if (tiempoActualMs - tiempoAnteriorMs >= SAMPLING_INTERVAL_MS)
  {
    tiempoAnteriorMs = tiempoActualMs;

    EstadoMedicion estado;
    // Lectura con filtro de mediana de 3 muestras (delay inter-muestra de 5 ms)
    float distancia = sensor.leerDistanciaFiltrada(3, 5, estado);

    #if CURRENT_OUTPUT_FORMAT == OUTPUT_FORMAT_HUMAN
      // Formato fiel al informe del laboratorio
      switch (estado)
      {
        case MEDICION_OK:
          Serial.print("Distancia: ");
          Serial.print(distancia, 2);
          Serial.println(" cm");
          break;

        case OBJETO_MUY_CERCA:
          Serial.println("Distancia: 0.00 cm");
          break;

        case FUERA_DE_RANGO:
        case SIN_ECO:
        default:
          Serial.println("SIN OBJETO");
          break;
      }

    #elif CURRENT_OUTPUT_FORMAT == OUTPUT_FORMAT_CSV
      // Formato CSV ideal para el data logger en Python: TimestampMs,DistanciaCm,Estado
      Serial.print(tiempoActualMs);
      Serial.print(",");
      Serial.print(distancia, 2);
      Serial.print(",");
      Serial.println(HCSR04Sensor::estadoAString(estado));

    #elif CURRENT_OUTPUT_FORMAT == OUTPUT_FORMAT_JSON
      // Formato JSON para microservicios e interfaces web
      Serial.print("{\"timestamp\":");
      Serial.print(tiempoActualMs);
      Serial.print(",\"distancia\":");
      Serial.print(distancia, 2);
      Serial.print(",\"estado\":\"");
      Serial.print(HCSR04Sensor::estadoAString(estado));
      Serial.println("\"}");

    #endif
  }
}
