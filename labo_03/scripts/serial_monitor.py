#!/usr/bin/env python3
"""
==============================================================================
UNSAAC - Robótica: Laboratorio N.° 03
ESP32 + HC-SR04: Monitor Serial Interactivo
==============================================================================
Script para leer y mostrar los datos del ESP32 en tiempo real desde la consola.
Detecta automáticamente puertos COM disponibles o permite seleccionar uno.
"""

import sys
import time
import argparse
from datetime import datetime

try:
    import serial
    import serial.tools.list_ports
except ImportError:
    print("[ERROR] Falta el paquete 'pyserial'. Instálalo con: pip install pyserial")
    sys.exit(1)


def listar_puertos():
    """Lista todos los puertos COM disponibles."""
    puertos = serial.tools.list_ports.comports()
    if not puertos:
        print("[AVISO] No se detectaron puertos seriales conectados.")
        return []
    print("\n--- Puertos COM Disponibles ---")
    for idx, p in enumerate(puertos):
        print(f" [{idx + 1}] {p.device} - {p.description}")
    return puertos


def seleccionar_puerto():
    """Permite al usuario elegir un puerto COM disponible interactiva o automáticamente."""
    puertos = listar_puertos()
    if not puertos:
        return None
    if len(puertos) == 1:
        print(f"[AUTO] Seleccionado automáticamente: {puertos[0].device}")
        return puertos[0].device

    while True:
        try:
            sel = input("\nSeleccione el número de puerto [1-{}]: ".format(len(puertos)))
            idx = int(sel) - 1
            if 0 <= idx < len(puertos):
                return puertos[idx].device
            print("Número fuera de rango.")
        except ValueError:
            print("Entrada inválida.")


def monitorear(puerto, baudrate=115200):
    """Abre el puerto serial y muestra las tramas recibidas con timestamp."""
    print(f"\n[INFO] Conectando a {puerto} a {baudrate} baudios...")
    try:
        ser = serial.Serial(puerto, baudrate, timeout=1.0)
        time.sleep(1.5)  # Espera para estabilizar el ESP32 tras reset
        print(f"[INFO] Conexión establecida exitosamente. Presione Ctrl+C para salir.\n")
        print("=" * 60)
        print(f"{'TIMESTAMP':<12} | {'SALIDA SERIAL':<45}")
        print("=" * 60)

        while True:
            linea = ser.readline().decode('utf-8', errors='replace').strip()
            if linea:
                hora = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                print(f"[{hora}] | {linea}")

    except KeyboardInterrupt:
        print("\n\n[INFO] Monitor serial detenido por el usuario.")
    except serial.SerialException as e:
        print(f"\n[ERROR] Error en comunicación serial: {e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("[INFO] Puerto serial cerrado.")


def main():
    parser = argparse.ArgumentParser(description="Monitor Serial para ESP32 + HC-SR04 (UNSAAC)")
    parser.add_argument("-p", "--port", help="Nombre del puerto serial (ej. COM3 o /dev/ttyUSB0)", default=None)
    parser.add_argument("-b", "--baud", type=int, help="Velocidad en baudios (por defecto 115200)", default=115200)
    args = parser.parse_args()

    puerto = args.port or seleccionar_puerto()
    if not puerto:
        print("[ERROR] No se pudo determinar un puerto serial válido.")
        sys.exit(1)

    monitorear(puerto, args.baud)


if __name__ == "__main__":
    main()
