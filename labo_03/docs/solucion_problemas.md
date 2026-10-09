# Guía de Solución de Problemas (Troubleshooting)
## Laboratorio 03: ESP32 + HC-SR04 | UNSAAC

Esta guía reúne los problemas más frecuentes experimentados durante el desarrollo del laboratorio, sus causas fundamentales y las soluciones probadas en el taller.

---

## 1. Problema de Carga: "Failed to connect to ESP32: Timed out waiting for packet header"

### Síntoma
Al presionar el botón "Subir" en Arduino IDE o PlatformIO, la consola muestra una serie de puntos y guiones `Connecting........_____.....` y finalmente aborta con error fatal de conexión.

### Causa Raíz
El circuito de reset automático del ESP32 DevKit (conformado por dos transistores bipolares y la señal DTR/RTS del convertidor USB-Serial CP2102/CH340) no logra colocar el pin `GPIO 0` en nivel bajo en el instante exacto en que la ROM entra en modo de programación (Bootloader).

### Solución Probada
1. Al momento de ver en consola el mensaje `Connecting........`, **mantenga presionado el botón "BOOT" (o "IO0")** en la placa ESP32 durante 2 a 3 segundos hasta que inicie el porcentaje de flasheo (`Writing at 0x00010000... [10%]`).
2. Luego suelte el botón. La placa se grabará con normalidad.
3. *Solución definitiva por hardware:* Soldar un condensador electrolítico o cerámico de $10\ \mu\text{F}$ entre los pines `EN` (Enable) y `GND` de la placa.

---

## 2. Monitor Serial Muestra Caracteres Ilegibles (Basura o "¿?")

### Síntoma
La consola serial imprime símbolos binarios extraños como `⸮⸮⸮x` en lugar del texto del programa.

### Causa Raíz
Discrepancia en la velocidad de transmisión en baudios (*baud rate*) entre el microcontrolador y el receptor de la PC.

### Solución
1. En la esquina inferior derecha del Monitor Serial de Arduino IDE, cambie la velocidad a **115200 baudios**.
2. Presione una vez el botón físico **EN** (o RESET) del ESP32 para reiniciar la ejecución y ver la cabecera completa con claridad.

---

## 3. Salida Serial Fija en "Distancia: 0.00 cm"

### Síntoma
El sensor siempre indica `0.00 cm` independientemente de si hay un objeto enfrente o no.

### Causa Raíz
El programa está leyendo una distancia $\le 3.0\text{ cm}$. Esto ocurre si:
* Los cables entre TRIG y ECHO están intercambiados.
* El pin ECHO está puenteado directamente a GND en el divisor resistivo.
* Hay un cable Dupont o componente físico colocado directamente delante de los conos ultrasónicos.

### Solución
1. Verifique que el pin **GPIO 5** esté conectado a **TRIG** y **GPIO 18** a **ECHO** (a través del divisor).
2. Inspeccione visualmente que el frente de las cápsulas ultrasónicas esté completamente despejado en un ángulo de al menos $30^\circ$.

---

## 4. Salida Serial Fija en "SIN OBJETO"

### Síntoma
El sensor siempre imprime `SIN OBJETO`, incluso colocando la mano a 15 cm frente a él.

### Causa Raíz
`pulseIn(ECHO_PIN, HIGH, TIMEOUT)` está devolviendo 0 por haberse vencido el tiempo de espera (30 ms). Esto sucede cuando:
1. El sensor no está recibiendo 5V en su pin VCC (el HC-SR04 requiere estrictamente 5V para que sus transductores alcancen la oscilación resonante a 40 kHz; con 3.3V no opera de manera confiable).
2. El pulso de disparo no se está generando (pin TRIG suelto o mal configurado).
3. La señal de retorno ECHO quedó interrumpida en el divisor resistivo.

### Solución
1. Mida con multímetro el voltaje entre el pin VCC y GND del sensor; debe medir entre **4.75V y 5.10V** (tomado del pin VIN alimentado por el USB).
2. Compruebe la continuidad de las resistencias del divisor ($1\text{ k}\Omega$ en serie y $2\text{ k}\Omega$ a masa).

---

## 5. Mediciones Inestables con Picos o Saltos Bruscos

### Síntoma
Al medir una distancia fija de 11 cm, la lectura salta aleatoriamente a 45 cm, 80 cm o "SIN OBJETO".

### Causa Raíz
* La superficie del objeto está inclinada respecto al sensor, haciendo que el haz sonoro se refleje en dirección oblicua y no regrese al receptor.
* Materiales fonoabsorbentes (telas, esponjas, peluche) que absorben la onda mecánica.
* Ruidos espurios o acoplamiento de ruidos de conmutación.

### Solución
1. Emplear un objeto con superficie plana, rígida y lisa (un cuaderno, caja de cartón o regla plana) orientado en ángulo recto ($90^\circ$) respecto al eje de los transductores.
2. Utilizar el firmware modular (`firmware/esp32_hcsr04_modular/`), el cual aplica un **filtro de mediana móvil** de 3 a 5 muestras para descartar rebotes parásitos y valores atípicos (*outliers*).
