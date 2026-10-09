#!/usr/bin/env python3
"""
==============================================================================
UNSAAC - Robótica: Laboratorio N.° 03
ESP32 + HC-SR04: Análisis Estadístico y Curva de Calibración
==============================================================================
Procesa las mediciones experimentales (Regla física vs HC-SR04),
calcula regresión lineal, error absoluto, error relativo porcentual,
MAE, RMSE y genera el gráfico de calibración y residuos.
Usa únicamente la biblioteca estándar (csv) y numpy / matplotlib.
"""

import os
import csv
import sys
import numpy as np
import matplotlib.pyplot as plt


def analizar_calibracion(ruta_csv="tests/datos_experimentales.csv", ruta_grafico="assets/images/grafico_calibracion.png"):
    if not os.path.exists(ruta_csv):
        print(f"[ERROR] Archivo {ruta_csv} no encontrado.")
        return

    x_nom_list = []
    y_med_list = []

    with open(ruta_csv, mode="r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for fila in reader:
            x = float(fila["Distancia_Nominal_Regla_cm"])
            y = float(fila["Distancia_Medida_Sensor_cm"])
            # Filtrar mediciones válidas para calibración lineal (excluyendo zona ciega <= 3cm)
            if x >= 5.0:
                x_nom_list.append(x)
                y_med_list.append(y)

    x_nom = np.array(x_nom_list)
    y_med = np.array(y_med_list)

    print("=" * 65)
    print("   ANÁLISIS ESTADÍSTICO DE CALIBRACIÓN: SENSOR HC-SR04 + ESP32   ")
    print("   Universidad Nacional de San Antonio Abad del Cusco (UNSAAC)   ")
    print("=" * 65)

    # Errores
    error_abs = y_med - x_nom
    error_rel_pct = (np.abs(error_abs) / x_nom) * 100.0

    # Regresión lineal por mínimos cuadrados: y = m * x + b
    n = len(x_nom)
    m, b = np.polyfit(x_nom, y_med, 1)

    y_pred = m * x_nom + b
    ss_tot = np.sum((y_med - np.mean(y_med)) ** 2)
    ss_res = np.sum((y_med - y_pred) ** 2)
    r2 = 1.0 - (ss_res / ss_tot)

    mae = np.mean(np.abs(error_abs))
    rmse = np.sqrt(np.mean(error_abs ** 2))

    print(f"\nNúmero de muestras analizadas (rango 5 a 100 cm): {n}")
    print(f"Ecuación de Regresión Lineal : d_sensor = {m:.4f} * d_regla + ({b:.4f})")
    print(f"Coeficiente de Determinación (R²): {r2:.6f}")
    print(f"Error Medio Absoluto (MAE)   : {mae:.4f} cm")
    print(f"Raíz del Error Cuadrático Medio (RMSE) : {rmse:.4f} cm")
    print(f"Error Relativo Promedio      : {np.mean(error_rel_pct):.2f} %")
    print(f"Error Relativo Máximo        : {np.max(error_rel_pct):.2f} % (a {x_nom[np.argmax(error_rel_pct)]} cm)")
    print("-" * 65)

    # Generación de la gráfica comparativa y de residuos
    os.makedirs(os.path.dirname(os.path.abspath(ruta_grafico)), exist_ok=True)
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(9, 8), gridspec_kw={"height_ratios": [2.5, 1]})

    # Subplot 1: Curva de Calibración
    ax1.scatter(x_nom, y_med, color="#0056b3", s=45, alpha=0.8, edgecolors="k", label="Muestras Experimentales (Sensor)", zorder=5)
    
    x_line = np.linspace(0, 105, 200)
    ax1.plot(x_line, x_line, "k--", alpha=0.5, label="Ideal Identidad ($y = x$)", zorder=3)
    ax1.plot(x_line, m * x_line + b, color="#e63946", linewidth=2.0, label=f"Ajuste Lineal ($y = {m:.4f}x + {b:.2f}$, $R^2={r2:.5f}$)", zorder=4)

    # Resaltar la medición representativa del informe (11.11 cm)
    ax1.scatter([11.0], [11.11], color="#2a9d8f", s=130, marker="*", label="Medición Clave Informe (11.11 cm)", zorder=6)

    ax1.set_title("Curva de Calibración: Distancia Medida (ESP32) vs Distancia Real (Regla)", fontsize=12, fontweight="bold")
    ax1.set_ylabel("Distancia Medida Sensor (cm)", fontsize=11)
    ax1.set_xlim(0, 105)
    ax1.set_ylim(0, 105)
    ax1.grid(True, linestyle=":", alpha=0.6)
    ax1.legend(loc="upper left", frameon=True)

    # Subplot 2: Residuos / Error Absoluto
    ax2.stem(x_nom, error_abs, linefmt="#2a9d8f", markerfmt="o", basefmt="k-")
    ax2.axhline(0, color="black", linestyle="-", linewidth=0.8)
    ax2.set_title("Residuo de Error Absoluto ($d_{sensor} - d_{regla}$)", fontsize=11, fontweight="bold")
    ax2.set_xlabel("Distancia Nominal con Regla (cm)", fontsize=11)
    ax2.set_ylabel("Error (cm)", fontsize=11)
    ax2.set_xlim(0, 105)
    ax2.grid(True, linestyle=":", alpha=0.6)

    plt.tight_layout()
    plt.savefig(ruta_grafico, dpi=300)
    plt.close()
    print(f"\n[ÉXITO] Gráfica guardada en: {os.path.abspath(ruta_grafico)}")
    print("=" * 65)


if __name__ == "__main__":
    analizar_calibracion()
