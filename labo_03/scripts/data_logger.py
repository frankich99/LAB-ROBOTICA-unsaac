#!/usr/bin/env python3
"""
==============================================================================
UNSAAC - Robótica: Laboratorio N.° 03
ESP32 + HC-SR04: Registrador de Datos a CSV (Data Logger)
==============================================================================
Captura mediciones enviadas por el ESP32 a través del puerto serie y las
almacena estructuradamente en un archivo CSV para posterior análisis estadístico.
"""

import os
import re
import sys
import csv
import time
import argparse
from datetime import datetime

try:
    import serial
    import serial.tools.list_ports
except ImportError:
    print("[ERROR] Falta el paquete 'pyserial'. Instálalo con: pip install pyserial")
    sys.exit(1)


PATRON_DISTANCIA = re.compile(r"Distancia:\s*([0-9.]+)\s*cm", re.IGNORECASE)
PATRON_SIN_OBJETO = re.compile(r"SIN OBJETO", re.IGNORECASE)


def parsear_linea(linea):
    """
    Parsea la línea recibida del ESP32.
    Soporta formato humano ('Distancia: 11.11 cm') o CSV ('timestamp,distancia,estado').
    Retorna tupla: (distancia_float_o_none, estado_str)
    """
    linea = linea.strip()
    if not linea:
        return None, None

    # Formato CSV directo: timestamp,dist,estado
    partes = linea.split(",")
    if len(partes) == 3:
        try:
            d = float(partes[1])
            return d, partes[2].strip()
        except ValueError:
            pass

    # Formato legible humano (informe oficial)
    match_dist = PATRON_DISTANCIA.search(linea)
    if match_dist:
        d = float(match_dist.group(1))
        if d == 0.0:
            return 0.0, "OBJETO_MUY_CERCA"
        return d, "MEDICION_OK"

    if PATRON_SIN_OBJETO.search(linea):
        return -1.0, "SIN_OBJETO"

    return None, linea


def iniciar_logger(puerto, baudrate, archivo_salida, max_muestras=None):
    os.makedirs(os.path.dirname(os.path.abspath(archivo_salida)), exist_ok=True)

    print(f"\n[INFO] Conectando a {puerto} ({baudrate} bps)...")
    try:
        ser = serial.Serial(puerto, baudrate, timeout=1.0)
    except serial.SerialException as e:
        print(f"[ERROR] No se pudo abrir {puerto}: {e}")
        return

    time.sleep(1.5)
    print(f"[INFO] Registrando en: {archivo_salida}")
    print("[INFO] Presione Ctrl+C para finalizar la captura.\n")

    existe_archivo = os.path.exists(archivo_salida) and os.path.getsize(archivo_salida) > 0

    muestras_guardadas = 0
    t_inicio = time.time()
    lecturas_validas = []

    with open(archivo_salida, mode="a", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        if not existe_archivo:
            writer.writerow(["Timestamp_ISO", "Tiempo_Relativo_s", "Distancia_cm", "Estado"])

        try:
            while True:
                linea = ser.readline().decode("utf-8", errors="replace").strip()
                if not linea:
                    continue

                dist, estado = parsear_linea(linea)
                if estado is None:
                    continue

                t_rel = round(time.time() - t_inicio, 3)
                timestamp_iso = datetime.now().isoformat()

                writer.writerow([timestamp_iso, t_rel, dist if dist is not None else "", estado])
                f.flush()

                muestras_guardadas += 1
                if dist is not None and dist > 0:
                    lecturas_validas.append(dist)

                print(f"[{t_rel:7.2f}s] Muestra #{muestras_guardadas:<4} | Dist: {str(dist) + ' cm':<12} | Estado: {estado}")

                if max_muestras and muestras_guardadas >= max_muestras:
                    print(f"\n[INFO] Se alcanzó el límite configurado de {max_muestras} muestras.")
                    break

        except KeyboardInterrupt:
            print("\n[INFO] Captura interrumpida por el usuario.")
        finally:
            ser.close()

    print("\n" + "=" * 50)
    print("           RESUMEN EXPERIMENTAL               ")
    print("=" * 50)
    print(f"Total de muestras capturadas : {muestras_guardadas}")
    print(f"Lecturas válidas registradas : {len(lecturas_validas)}")
    if lecturas_validas:
        media = sum(lecturas_validas) / len(lecturas_validas)
        varianza = sum((x - media) ** 2 for x in lecturas_validas) / len(lecturas_validas)
        desv = varianza ** 0.5
        print(f"Distancia Promedio           : {media:.2f} cm")
        print(f"Desviación Estándar (σ)      : {desv:.4f} cm")
        print(f"Valor Mínimo                 : {min(lecturas_validas):.2f} cm")
        print(f"Valor Máximo                 : {max(lecturas_validas):.2f} cm")
    print(f"Archivo guardado en          : {os.path.abspath(archivo_salida)}")
    print("=" * 50)


def main():
    parser = argparse.ArgumentParser(description="Data Logger para ESP32 + HC-SR04")
    parser.add_argument("-p", "--port", required=True, help="Puerto serial (ej. COM3, COM4)")
    parser.add_argument("-b", "--baud", type=int, default=115200, help="Velocidad en baudios")
    parser.add_argument("-o", "--output", default="data/mediciones_experimento.csv", help="Ruta del archivo CSV destino")
    parser.add_argument("-n", "--num-samples", type=int, default=None, help="Número de muestras a registrar (opcional)")
    args = parser.parse_args()

    iniciar_logger(args.port, args.baud, args.output, args.num_samples)


if __name__ == "__main__":
    main()
