// ==============================================================================
// UNIVERSIDAD NACIONAL DE SAN ANTONIO ABAD DEL CUSCO (UNSAAC)
// FACULTAD DE INGENIERÍA ELÉCTRICA, ELECTRÓNICA, INFORMÁTICA Y MECÁNICA
// ESCUELA PROFESIONAL DE INGENIERÍA INFORMÁTICA Y DE SISTEMAS
//
// ASIGNATURA : Robótica
// PRÁCTICA   : Laboratorio N.° 02 - Puesta en Marcha y Configuración del ESP32
// PRUEBA 03  : Evaluación del Transceptor Wi-Fi Integrado (2.4 GHz) - Escaneo
// ESTUDIANTE : Noe Franklin Choquenaira Quispe (Cód. 133962)
// PLACA      : DOIT ESP32 DEVKIT V1 / NodeMCU V1 (30 pines)
// ==============================================================================

#include "WiFi.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==================================================");
  Serial.println("  UNSAAC - Lab 02: ESP32 NodeMCU V1               ");
  Serial.println("  Prueba 3: Escaneo de Redes Wi-Fi (2.4 GHz)      ");
  Serial.println("==================================================");

  // Configurar Wi-Fi en modo Estación (WIFI_STA) y desconectar de cualquier AP previo
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Iniciando escaneo de canales Wi-Fi 1 al 13...");
}

void loop()
{
  Serial.println("\n[INFO] Realizando escaneo de redes disponibles...");

  // WiFi.scanNetworks devuelve el número de redes encontradas
  int n = WiFi.scanNetworks();
  Serial.println("[INFO] Escaneo finalizado.");

  if (n == 0)
  {
    Serial.println("No se detectaron redes inalámbricas en el entorno.");
  }
  else
  {
    Serial.print("Se encontraron ");
    Serial.print(n);
    Serial.println(" redes:");
    Serial.println("--------------------------------------------------");
    Serial.printf("%-4s | %-25s | %-8s | %-5s\n", "N°", "SSID", "RSSI", "CANAL");
    Serial.println("--------------------------------------------------");

    for (int i = 0; i < n; ++i)
    {
      Serial.printf("%-4d | %-25.25s | %-5d dBm | Ch %-3d\n", 
                    i + 1, 
                    WiFi.SSID(i).c_str(), 
                    WiFi.RSSI(i), 
                    WiFi.channel(i));
      delay(10);
    }
    Serial.println("--------------------------------------------------");
  }

  // Esperar 10 segundos antes del siguiente ciclo de escaneo
  delay(10000);
}
