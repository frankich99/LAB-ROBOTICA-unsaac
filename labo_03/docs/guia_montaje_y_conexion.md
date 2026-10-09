# Guía de Montaje Físico y Conexión Eléctrica
## Sistema ESP32 + HC-SR04 | Laboratorio de Robótica

Esta guía proporciona las instrucciones detalladas paso a paso para ensamblar de forma segura y correcta el circuito de medición de distancia sobre el protoboard, protegiendo las entradas digitales del ESP32 contra niveles destructivos de voltaje.

---

## 1. Materiales Requeridos

* **1x** Módulo microcontrolador **ESP32 DevKit V1** (30 pines o 38 pines)
* **1x** Sensor ultrasónico **HC-SR04**
* **1x** Protoboard de 830 puntos
* **1x** Resistencia de **$1.0\text{ k}\Omega$** (Marrón - Negro - Rojo - Dorado)
* **1x** Resistencia de **$2.0\text{ k}\Omega$** o **$2.2\text{ k}\Omega$** (Rojo - Rojo - Rojo - Dorado)
* **6x** Cables Dupont macho-macho (de preferencia colores normalizados: Rojo = 5V, Negro = GND, Azul = TRIG, Amarillo/Verde = ECHO)
* **1x** Cable de datos Micro-USB a USB-A (o Tipo-C según el modelo de placa)
* **1x** Regla métrica milimetrada para pruebas y calibración física

---

## 2. Diagrama de Pines de Referencia (ESP32 DevKit)

![Pinout ESP32](assets/images/esp32_pinout.png)

### Asignación de Pines Utilizada

| Pin del Sensor | Pin del ESP32 | Nivel de Voltaje | Función Técnica |
| :---: | :---: | :---: | :--- |
| **VCC** | **VIN / 5V** | 5.0 V DC | Alimentación del circuito emisor y receptor ultrasónico |
| **GND** | **GND** | 0.0 V | Masa o tierra de referencia común |
| **TRIG** | **GPIO 5** | 3.3 V CMOS | Pulso de excitación/disparo enviado por el ESP32 |
| **ECHO** | **GPIO 18** *(vía divisor)* | 3.3 V CMOS | Pulso de retorno que indica el tiempo de vuelo |

---

## 3. Esquema Eléctrico y Adaptación de Nivel Lógico (Divisor 5V -> 3.3V)

![Esquema de Conexión](assets/images/esquema_conexion_esp32_hcsr04.png)

```
        +5V (ESP32 VIN) -------------------- [VCC] Sensor HC-SR04
        
        GND (ESP32) ------------------------ [GND] Sensor HC-SR04
        
        GPIO 5 (ESP32) --------------------- [TRIG] Sensor HC-SR04
                                                     
                                             [ECHO] Sensor HC-SR04 (Salida 5V TTL)
                                               |
                                            [ R1: 1.0 kΩ ]
                                               |
        GPIO 18 (ESP32) <----------------------+   (Tensión atenuada a ~3.3V)
                                               |
                                            [ R2: 2.0 kΩ ó 2.2 kΩ ]
                                               |
        GND (ESP32) <--------------------------+
```

### Cálculo de Validación:
$$V_{out} = 5.0\text{ V} \times \frac{2000\,\Omega}{1000\,\Omega + 2000\,\Omega} = 5.0 \times \frac{2}{3} \approx 3.33\text{ V}$$

*Nivel completamente seguro para las entradas digitales CMOS del ESP32 ($V_{IH_{max}} = 3.6\text{ V}$).*

---

## 4. Procedimiento de Ensamblaje Paso a Paso

### Paso 1: Inserción del ESP32 en el Protoboard
1. Ubique el módulo ESP32 a horcajadas sobre la ranura divisoria central del protoboard, preferentemente hacia un extremo.
2. Presione firmemente y de manera uniforme en ambos lados para evitar doblar los pines.
3. Verifique que quede al menos una fila libre de orificios a cada lado para facilitar las conexiones.

### Paso 2: Fijación del Sensor HC-SR04
1. Inserte los 4 pines del HC-SR04 (`VCC`, `TRIG`, `ECHO`, `GND`) en el extremo opuesto del protoboard.
2. Asegúrese de que las dos "cápsulas" (transductores) apunten hacia afuera, libres de cables que puedan provocar rebotes acústicos parásitos.

### Paso 3: Conexión de Alimentación
1. Conecte un cable Dupont desde el pin **VIN** (o **5V**) del ESP32 al riel positivo rojo del protoboard.
2. Conecte un cable Dupont desde cualquier pin **GND** del ESP32 al riel negativo azul/negro del protoboard.
3. Conecte el pin **VCC** del HC-SR04 al riel positivo (+5V).
4. Conecte el pin **GND** del HC-SR04 al riel negativo (GND).

### Paso 4: Conexión de Disparo (TRIG)
1. Conecte un cable directo desde el pin **GPIO 5** del ESP32 hasta el pin **TRIG** del sensor.

### Paso 5: Conexión del Divisor de Voltaje para Retorno (ECHO)
1. Conecte un extremo de la resistencia $R_1$ ($1\text{ k}\Omega$) a la misma fila del pin **ECHO** del sensor.
2. Inserte el otro extremo de $R_1$ en un nodo intermedio libre del protoboard.
3. Desde ese nodo intermedio:
   * Conecte un cable hacia el **GPIO 18** del ESP32.
   * Conecte la resistencia $R_2$ ($2.0\text{ k}\Omega$ o $2.2\text{ k}\Omega$) entre ese nodo y el riel de **GND**.

---

## 5. Lista de Verificación Previa a la Conexión USB (Checklist)

- [ ] ¿La masa (GND) del ESP32 y la del sensor están unidas en el mismo riel?
- [ ] ¿El pin ECHO pasa a través de la resistencia divisoria antes de llegar a GPIO 18?
- [ ] ¿TRIG está conectado a GPIO 5 sin cortocircuitar pines contiguos?
- [ ] ¿El sensor apunta hacia un espacio despejado sin cables bloqueando los transductores?
- [ ] ¿El cable USB transmite tanto alimentación como líneas de datos D+ / D-?
