#!/usr/bin/env python3
"""
==============================================================================
UNSAAC - Robótica: Laboratorio N.° 03
ESP32 + HC-SR04: Graficador en Tiempo Real (Live Plotter)
==============================================================================
Visualizador gráfico interactivo utilizando Matplotlib. Muestra la curva
temporal de distancia, umbrales de zona ciega y rango práctico en tiempo real.
Incluye modo demostración (--demo) para pruebas sin hardware físico conectado.
"""

import sys
import time
import math
import argparse
import threading
from collections import deque

try:
    import matplotlib.pyplot as plt
    import matplotlib.animation as animation
except ImportError:
    print("[ERROR] Falta el paquete 'matplotlib'. Instálalo con: pip install matplotlib")
    sys.exit(1)


# Tamaño de la ventana deslizante de muestras
MAX_PUNTOS = 100
HIST_TIEMPO = deque(maxlen=MAX_PUNTOS)
HIST_DIST = deque(maxlen=MAX_PUNTOS)
HIST_ESTADO = deque(maxlen=MAX_PUNTOS)

lock_datos = threading.Lock()
ejecutando = True


def hilo_serial(puerto, baudrate):
    """Hilo secundario para lectura no bloqueante del puerto serial."""
    global ejecutando
    try:
        import serial
        ser = serial.Serial(puerto, baudrate, timeout=1.0)
        time.sleep(1.5)
        t0 = time.time()
        from scripts.data_logger import parsear_linea
    except Exception as e:
        print(f"[ERROR] No se pudo conectar a {puerto}: {e}")
        ejecutando = False
        return

    while ejecutando:
        try:
            linea = ser.readline().decode("utf-8", errors="replace").strip()
            if not linea:
                continue
            dist, estado = parsear_linea(linea)
            if estado is not None:
                t_rel = time.time() - t0
                with lock_datos:
                    HIST_TIEMPO.append(t_rel)
                    HIST_DIST.append(dist if (dist is not None and dist >= 0) else 0.0)
                    HIST_ESTADO.append(estado)
        except Exception:
            break

    ser.close()


def hilo_demo():
    """Generador sintético de datos para probar la interfaz sin hardware."""
    global ejecutando
    t0 = time.time()
    while ejecutando:
        t_rel = time.time() - t0
        # Simula un objeto oscilando entre 5 cm y 45 cm
        dist = 25.0 + 20.0 * math.sin(t_rel * 1.5)
        estado = "MEDICION_OK"
        if dist < 3.0:
            dist = 0.0
            estado = "OBJETO_MUY_CERCA"

        with lock_datos:
            HIST_TIEMPO.append(t_rel)
            HIST_DIST.append(dist)
            HIST_ESTADO.append(estado)

        time.sleep(0.08)


def main():
    global ejecutando

    parser = argparse.ArgumentParser(description="Live Plotter para ESP32 + HC-SR04")
    parser.add_argument("-p", "--port", help="Puerto serial (ej. COM3)")
    parser.add_argument("-b", "--baud", type=int, default=115200, help="Velocidad en baudios")
    parser.add_argument("--demo", action="store_true", help="Ejecutar en modo simulación/demo")
    args = parser.parse_args()

    if args.demo or not args.port:
        if not args.demo:
            print("[INFO] No se especificó puerto serial (-p). Iniciando en modo DEMO (--demo)...")
        th = threading.Thread(target=hilo_demo, daemon=True)
    else:
        th = threading.Thread(target=hilo_serial, args=(args.port, args.baud), daemon=True)

    th.start()

    # Configuración de la figura Matplotlib
    plt.style.use("seaborn-v0_8-darkgrid" if "seaborn-v0_8-darkgrid" in plt.style.available else "default")
    fig, ax = plt.subplots(figsize=(10, 5.5))
    fig.canvas.manager.set_window_title("UNSAAC - Robótica: Telemetría HC-SR04 + ESP32")

    linea_plot, = ax.plot([], [], color="#007acc", linewidth=2.0, label="Distancia Medida (cm)")
    punto_actual, = ax.plot([], [], marker="o", markersize=8, color="#d9534f")

    # Líneas de referencia de umbrales
    ax.axhline(y=3.0, color="#d9534f", linestyle="--", linewidth=1.2, label="Umbral Zona Ciega (3.0 cm)")
    ax.axhline(y=100.0, color="#f0ad4e", linestyle="--", linewidth=1.2, label="Rango Práctico Máx (100 cm)")

    ax.set_title("Monitoreo en Tiempo Real: Sensor Ultrasónico HC-SR04 + ESP32", fontsize=13, fontweight="bold", pad=12)
    ax.set_xlabel("Tiempo Relativo (segundos)", fontsize=11)
    ax.set_ylabel("Distancia (cm)", fontsize=11)
    ax.set_ylim(-5, 115)
    ax.legend(loc="upper right", frameon=True)
    ax.grid(True, linestyle=":", alpha=0.6)

    texto_info = ax.text(0.02, 0.95, "", transform=ax.transAxes, fontsize=11,
                         verticalalignment="top", bbox=dict(boxstyle="round,pad=0.5", facecolor="white", alpha=0.85, edgecolor="#ccc"))

    def actualizar(frame):
        with lock_datos:
            if not HIST_TIEMPO:
                return linea_plot, punto_actual, texto_info

            tiempos = list(HIST_TIEMPO)
            distancias = list(HIST_DIST)
            estados = list(HIST_ESTADO)

        linea_plot.set_data(tiempos, distancias)

        ult_t = tiempos[-1]
        ult_d = distancias[-1]
        ult_est = estados[-1]

        punto_actual.set_data([ult_t], [ult_d])

        t_min = max(0, ult_t - 15)
        t_max = max(15, ult_t + 2)
        ax.set_xlim(t_min, t_max)

        texto_info.set_text(f"Distancia Actual: {ult_d:.2f} cm\nEstado: {ult_est}\nMuestra: #{len(tiempos)}")

        return linea_plot, punto_actual, texto_info

    ani = animation.FuncAnimation(fig, actualizar, interval=50, blit=False)

    try:
        plt.show()
    except KeyboardInterrupt:
        pass
    finally:
        ejecutando = False


if __name__ == "__main__":
    main()
