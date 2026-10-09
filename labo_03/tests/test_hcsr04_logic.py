#!/usr/bin/env python3
"""
==============================================================================
UNSAAC - Robótica: Laboratorio N.° 03
ESP32 + HC-SR04: Pruebas Unitarias de Lógica y Algoritmos
==============================================================================
Verifica de forma automatizada las fórmulas físicas de tiempo de vuelo,
compensación térmica, umbrales de zona ciega y el filtro de mediana.
Compatible con unittest (estándar) y pytest.
"""

import unittest
import math


def calcular_velocidad_sonido(temp_celsius):
    """v(T) = (331.3 + 0.606 * T) * 1e-4 cm/µs"""
    return (331.3 + 0.606 * temp_celsius) * 0.0001


def calcular_distancia(tiempo_us, vel_sonido=0.0343, dist_cero=3.0, dist_max=100.0):
    """
    Replica la lógica de medición implementada en el firmware del ESP32:
    - Retorna -1.0 si tiempo_us == 0
    - Retorna 0.00 si d <= dist_cero
    - Retorna -1.0 (o None) si d > dist_max
    - Retorna d en caso normal
    """
    if tiempo_us == 0:
        return -1.0, "SIN_OBJETO"

    d = (tiempo_us * vel_sonido) / 2.0

    if d <= dist_cero:
        return 0.00, "OBJETO_MUY_CERCA"
    elif d <= dist_max:
        return round(d, 2), "MEDICION_OK"
    else:
        return -1.0, "SIN_OBJETO"


def filtro_mediana(muestras):
    """Calcula la mediana estadística de una lista de muestras numéricas."""
    if not muestras:
        return None
    ordenadas = sorted(muestras)
    n = len(ordenadas)
    mid = n // 2
    if n % 2 == 1:
        return ordenadas[mid]
    else:
        return (ordenadas[mid - 1] + ordenadas[mid]) / 2.0


class TestHCSR04Logica(unittest.TestCase):

    def test_tiempo_vuelo_nominal(self):
        """Verifica el cálculo de distancia con el caso real del informe (11.11 cm)."""
        # Para 11.11 cm: t = (2 * d) / 0.0343 = (2 * 11.11) / 0.0343 ≈ 647.81 µs
        tiempo_eco_us = 647.8134
        dist, estado = calcular_distancia(tiempo_eco_us)
        self.assertEqual(estado, "MEDICION_OK")
        self.assertAlmostEqual(dist, 11.11, places=2)

    def test_objeto_muy_cerca_zona_ciega(self):
        """Lecturas <= 3.0 cm deben reportarse como 0.00 cm según especificación del informe."""
        # Objeto a 1.5 cm -> t ≈ (2 * 1.5) / 0.0343 = 87.46 µs
        tiempo_eco_us = 87.46
        dist, estado = calcular_distancia(tiempo_eco_us)
        self.assertEqual(estado, "OBJETO_MUY_CERCA")
        self.assertEqual(dist, 0.00)

        # Objeto exactamente en el límite de 3.0 cm
        tiempo_3cm = (2.0 * 3.0) / 0.0343
        dist_3, estado_3 = calcular_distancia(tiempo_3cm)
        self.assertEqual(estado_3, "OBJETO_MUY_CERCA")
        self.assertEqual(dist_3, 0.00)

    def test_sin_objeto_timeout(self):
        """Cuando pulseIn retorna 0 (timeout), debe reportar SIN_OBJETO y distancia -1.0."""
        dist, estado = calcular_distancia(0)
        self.assertEqual(dist, -1.0)
        self.assertEqual(estado, "SIN_OBJETO")

    def test_fuera_de_rango_maximo(self):
        """Distancias superiores a 100 cm deben clasificarse como SIN_OBJETO en el laboratorio."""
        # Objeto a 150 cm -> t = (2 * 150) / 0.0343 = 8746.35 µs
        tiempo_lejos = 8746.35
        dist, estado = calcular_distancia(tiempo_lejos)
        self.assertEqual(dist, -1.0)
        self.assertEqual(estado, "SIN_OBJETO")

    def test_compensacion_termica(self):
        """Verifica la fórmula empírica de velocidad del sonido en aire seco."""
        # A 0 °C: v ≈ 331.3 m/s = 0.03313 cm/µs
        v_0 = calcular_velocidad_sonido(0.0)
        self.assertAlmostEqual(v_0, 0.03313, places=5)

        # A 20 °C: v = 331.3 + 12.12 = 343.42 m/s = 0.034342 cm/µs
        v_20 = calcular_velocidad_sonido(20.0)
        self.assertAlmostEqual(v_20, 0.034342, places=5)

    def test_filtro_mediana_rechazo_outliers(self):
        """Verifica que el filtro de mediana descarte lecturas anómalas (outliers o rebotes espurios)."""
        muestras_con_ruido = [11.11, 11.10, 85.40, 11.12, 11.09]  # 85.40 es ruido espurio
        mediana = filtro_mediana(muestras_con_ruido)
        self.assertAlmostEqual(mediana, 11.11, places=2)


if __name__ == "__main__":
    unittest.main(verbosity=2)
