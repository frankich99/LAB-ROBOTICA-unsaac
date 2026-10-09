#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==============================================================================
// CONFIGURACIÓN DE PINES HARDWARE (ESP32)
// ==============================================================================
// TRIG: Pin digital configurado como SALIDA para enviar el tren de pulsos (GPIO 5)
#define PIN_TRIG 5

// ECHO: Pin digital configurado como ENTRADA para medir el tiempo de retorno (GPIO 18)
// NOTA IMPORTANTE: La señal ECHO del sensor entrega 5V. Se debe utilizar un divisor
// de tensión resistivo (ej. 1kΩ y 2kΩ) para adecuar el nivel lógico a los 3.3V del ESP32.
#define PIN_ECHO 18

// ==============================================================================
// PARÁMETROS FÍSICOS Y DE CALIBRACIÓN
// ==============================================================================
// Temperatura ambiente estimada en grados Celsius (°C) en el laboratorio (Cusco ~18°C)
const float TEMPERATURA_AMBIENTE_C = 18.0f;

// Velocidad del sonido calculada dinámicamente según la temperatura:
// v(T) = 331.3 + (0.606 * T) [m/s]
// En cm/µs: v_us = (331.3 + 0.606 * T) * 100 / 1000000 = (331.3 + 0.606 * T) * 0.0001
inline float calcularVelocidadSonido(float tempC) {
    return (331.3f + (0.606f * tempC)) * 0.0001f; // ~0.03422 cm/µs a 18°C
}

// Velocidad por defecto estándar a 20°C: 0.0343 cm/µs
const float VELOCIDAD_SONIDO_DEFAULT = 0.0343f;

// ==============================================================================
// RANGOS Y UMBRALES DE MEDICIÓN
// ==============================================================================
// Umbral de zona ciega inferior (cm). Transductores saturados por rebote inmediato.
const float DISTANCIA_MIN_CM = 3.0f;

// Rango práctico máximo de medición experimental (cm).
const float DISTANCIA_MAX_CM = 100.0f;

// Tiempo máximo de espera para pulso ECHO en microsegundos (µs)
// 30,000 µs equivale a una distancia teórica de ida y vuelta de ~514 cm (~257 cm unidireccional)
const unsigned long ECHO_TIMEOUT_US = 30000UL;

// Duración del pulso de disparo acústico (estándar del fabricante: 10 µs)
const unsigned int TRIGGER_PULSE_US = 10;

// ==============================================================================
// COMUNICACIÓN SERIAL Y TEMPORIZACIÓN
// ==============================================================================
const unsigned long SERIAL_BAUDRATE = 115200;

// Período de muestreo no bloqueante en milisegundos (ms)
const unsigned long SAMPLING_INTERVAL_MS = 100;

// Número de muestras para el filtro de promediado o mediana (1 = sin filtro)
const uint8_t FILTER_SAMPLES = 5;

// Formato de salida serial:
// 0 = Formato legible humano (estilo informe de laboratorio)
// 1 = Formato CSV (Timestamp_ms,Distancia_cm,Estado)
// 2 = Formato JSON para integración IoT
#define OUTPUT_FORMAT_HUMAN 0
#define OUTPUT_FORMAT_CSV   1
#define OUTPUT_FORMAT_JSON  2

#define CURRENT_OUTPUT_FORMAT OUTPUT_FORMAT_HUMAN

#endif // CONFIG_H
