// ==============================================================================
// UNIVERSIDAD NACIONAL DE SAN ANTONIO ABAD DEL CUSCO (UNSAAC)
// FACULTAD DE INGENIERÍA ELÉCTRICA, ELECTRÓNICA, INFORMÁTICA Y MECÁNICA
// ESCUELA PROFESIONAL DE INGENIERÍA INFORMÁTICA Y DE SISTEMAS
//
// ASIGNATURA : Robótica
// PRÁCTICA   : Laboratorio N.° 02 - Puesta en Marcha y Configuración del ESP32
// PRUEBA 01  : Control Digital y Parpadeo de LED (Blink) con Telemetría Serial
// ESTUDIANTE : Noe Franklin Choquenaira Quispe (Cód. 133962)
// PLACA      : DOIT ESP32 DEVKIT V1 / NodeMCU V1 (30 pines)
// ==============================================================================

#define LED_PIN 2  // LED azul integrado en la placa (GPIO 2)

void setup()
{
  // Inicialización de la consola UART a 115200 baudios
  Serial.begin(115200);

  // Configuración del GPIO 2 como salida digital
  pinMode(LED_PIN, OUTPUT);

  delay(500);
  Serial.println("==================================================");
  Serial.println("  UNSAAC - Lab 02: ESP32 NodeMCU V1               ");
  Serial.println("  Prueba 1: Control de GPIO 2 (LED Integrado)     ");
  Serial.println("==================================================");
}

void loop()
{
  // Encender LED integrado y reportar estado por serial
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED Encendido");
  delay(1000);

  // Apagar LED integrado y reportar estado por serial
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED Apagado");
  delay(1000);
}
