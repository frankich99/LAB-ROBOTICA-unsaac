#include "HCSR04.h"

HCSR04Sensor::HCSR04Sensor(uint8_t pinTrig, uint8_t pinEcho, unsigned long timeoutUs,
                           float distMin, float distMax, float velSonido)
    : _pinTrig(pinTrig), _pinEcho(pinEcho), _timeoutUs(timeoutUs),
      _distanciaMin(distMin), _distanciaMax(distMax), _velocidadSonido(velSonido)
{
}

void HCSR04Sensor::begin()
{
    pinMode(_pinTrig, OUTPUT);
    pinMode(_pinEcho, INPUT);
    digitalWrite(_pinTrig, LOW);
    delay(200); // Estabilización eléctrica del transductor
}

unsigned long HCSR04Sensor::dispararYMideTiempo()
{
    // Estabilización previa de la línea TRIG
    digitalWrite(_pinTrig, LOW);
    delayMicroseconds(2);

    // Pulso de disparo ultrasónico exacto de 10 microsegundos
    digitalWrite(_pinTrig, HIGH);
    delayMicroseconds(TRIGGER_PULSE_US);
    digitalWrite(_pinTrig, LOW);

    // Medición del ancho de pulso en alto generado en ECHO
    unsigned long duracionUs = pulseIn(_pinEcho, HIGH, _timeoutUs);
    return duracionUs;
}

float HCSR04Sensor::leerDistancia(EstadoMedicion &estado)
{
    unsigned long tiempoUs = dispararYMideTiempo();

    // Caso 1: No hubo eco de retorno dentro de la ventana de tiempo
    if (tiempoUs == 0)
    {
        estado = SIN_ECO;
        return -1.0f;
    }

    // Cálculo físico: d = (t * v) / 2
    float d = (tiempoUs * _velocidadSonido) / 2.0f;

    // Caso 2: Objeto pegado al transductor (zona ciega <= 3.0 cm)
    if (d <= _distanciaMin)
    {
        estado = OBJETO_MUY_CERCA;
        return 0.0f;
    }

    // Caso 3: Fuera del rango de trabajo práctico útil
    if (d > _distanciaMax)
    {
        estado = FUERA_DE_RANGO;
        return d;
    }

    // Caso 4: Medición válida normal
    estado = MEDICION_OK;
    return d;
}

float HCSR04Sensor::leerDistanciaFiltrada(uint8_t n, unsigned long delayMuestrasMs, EstadoMedicion &estado)
{
    if (n < 1) n = 1;
    if (n > 15) n = 15;

    float buffer[15];
    uint8_t validCount = 0;
    EstadoMedicion ultimoEstado = SIN_ECO;

    for (uint8_t i = 0; i < n; i++)
    {
        EstadoMedicion st;
        float val = leerDistancia(st);
        ultimoEstado = st;

        if (st == MEDICION_OK || st == OBJETO_MUY_CERCA)
        {
            buffer[validCount++] = val;
        }

        if (i < n - 1)
        {
            delay(delayMuestrasMs);
        }
    }

    // Si la mayoría de muestras no captaron eco
    if (validCount == 0)
    {
        estado = ultimoEstado;
        return (ultimoEstado == OBJETO_MUY_CERCA) ? 0.0f : -1.0f;
    }

    // Ordenamiento por inserción para obtener la mediana estadística (robusto a outliers)
    for (uint8_t i = 1; i < validCount; i++)
    {
        float key = buffer[i];
        int j = i - 1;
        while (j >= 0 && buffer[j] > key)
        {
            buffer[j + 1] = buffer[j];
            j--;
        }
        buffer[j + 1] = key;
    }

    float mediana = buffer[validCount / 2];

    if (mediana <= _distanciaMin)
    {
        estado = OBJETO_MUY_CERCA;
        return 0.0f;
    }

    estado = MEDICION_OK;
    return mediana;
}

void HCSR04Sensor::setTemperaturaAmbiente(float tempCelsius)
{
    _velocidadSonido = calcularVelocidadSonido(tempCelsius);
}

const char* HCSR04Sensor::estadoAString(EstadoMedicion estado)
{
    switch (estado)
    {
        case MEDICION_OK:
            return "MEDICION_OK";
        case OBJETO_MUY_CERCA:
            return "OBJETO_MUY_CERCA";
        case FUERA_DE_RANGO:
            return "FUERA_DE_RANGO";
        case SIN_ECO:
            return "SIN_OBJETO";
        default:
            return "DESCONOCIDO";
    }
}
