#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>
#include "config.h"

// Estados posibles de la medición
enum EstadoMedicion {
    MEDICION_OK,            // Medición válida y dentro del rango normal
    OBJETO_MUY_CERCA,       // Objeto pegado al sensor (zona ciega <= DISTANCIA_MIN_CM)
    FUERA_DE_RANGO,         // Medición superior a DISTANCIA_MAX_CM
    SIN_ECO                 // Timeout superado, no hay obstáculo reflectivo detectable
};

class HCSR04Sensor {
private:
    uint8_t _pinTrig;
    uint8_t _pinEcho;
    unsigned long _timeoutUs;
    float _distanciaMin;
    float _distanciaMax;
    float _velocidadSonido;

    // Métodos internos
    unsigned long dispararYMideTiempo();

public:
    /**
     * @brief Constructor del controlador del sensor HC-SR04
     * @param pinTrig Pin GPIO conectado al pin TRIG
     * @param pinEcho Pin GPIO conectado al pin ECHO (con divisor resistivo)
     * @param timeoutUs Tiempo máximo de espera en microsegundos
     * @param distMin Distancia mínima en cm (zona ciega)
     * @param distMax Distancia máxima en cm
     * @param velSonido Velocidad del sonido en cm/µs
     */
    HCSR04Sensor(uint8_t pinTrig = PIN_TRIG, 
                 uint8_t pinEcho = PIN_ECHO, 
                 unsigned long timeoutUs = ECHO_TIMEOUT_US,
                 float distMin = DISTANCIA_MIN_CM,
                 float distMax = DISTANCIA_MAX_CM,
                 float velSonido = VELOCIDAD_SONIDO_DEFAULT);

    /**
     * @brief Inicializa los pines GPIO y estabiliza el transductor
     */
    void begin();

    /**
     * @brief Realiza una lectura instantánea única
     * @param[out] estado Referencia donde se almacena el estado diagnóstico resultante
     * @return Distancia calculada en cm (o -1.0 si falla / sin eco)
     */
    float leerDistancia(EstadoMedicion &estado);

    /**
     * @brief Realiza múltiples lecturas y calcula la mediana para filtrar ruido espurio
     * @param n Cantidad de muestras a tomar (mínimo 1, máximo 15)
     * @param delayMuestrasMs Intervalo entre lecturas sucesivas del filtro
     * @param[out] estado Estado diagnóstico resultante de la mediana
     * @return Distancia filtrada en cm
     */
    float leerDistanciaFiltrada(uint8_t n, unsigned long delayMuestrasMs, EstadoMedicion &estado);

    /**
     * @brief Actualiza la velocidad del sonido según la temperatura ambiente
     * @param tempCelsius Temperatura ambiente en grados Celsius
     */
    void setTemperaturaAmbiente(float tempCelsius);

    /**
     * @brief Convierte un estado de medición a una cadena de texto legible
     */
    static const char* estadoAString(EstadoMedicion estado);
};

#endif // HCSR04_H
