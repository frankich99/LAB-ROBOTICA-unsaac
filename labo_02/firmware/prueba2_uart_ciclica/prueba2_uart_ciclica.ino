// ==============================================================================
// UNIVERSIDAD NACIONAL DE SAN ANTONIO ABAD DEL CUSCO (UNSAAC)
// FACULTAD DE INGENIERÍA ELÉCTRICA, ELECTRÓNICA, INFORMÁTICA Y MECÁNICA
// ESCUELA PROFESIONAL DE INGENIERÍA INFORMÁTICA Y DE SISTEMAS
//
// ASIGNATURA : Robótica
// PRÁCTICA   : Laboratorio N.° 02 - Puesta en Marcha y Configuración del ESP32
// PRUEBA 02  : Transmisión UART Continua y Verificación de Estabilidad
// ESTUDIANTE : Noe Franklin Choquenaira Quispe (Cód. 133962)
// PLACA      : DOIT ESP32 DEVKIT V1 / NodeMCU V1 (30 pines)
// ==============================================================================

void setup()
{
  // Configuración de la velocidad de transmisión serial a 115200 bps
  Serial.begin(115200);

  // Pausa de estabilización tras reset del puente CP2102
  delay(1000);

  Serial.println("==================================================");
  Serial.println("  UNSAAC - Lab 02: ESP32 NodeMCU V1               ");
  Serial.println("  Prueba 2: Telemetría UART Cíclica (1 Hz)        ");
  Serial.println("==================================================");
  Serial.println("ESP32 iniciado correctamente");
}

void loop()
{
  // Envío periódico continuo de latido (heartbeat)
  Serial.println("ESP32 funcionando");
  delay(1000);
}
