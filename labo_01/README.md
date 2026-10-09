# UNIVERSIDAD NACIONAL DE SAN ANTONIO ABAD DEL CUSCO
### FACULTAD DE INGENIERÍA ELÉCTRICA, ELECTRÓNICA, INFORMÁTICA Y MECÁNICA
**ESCUELA PROFESIONAL DE INGENIERÍA INFORMÁTICA Y DE SISTEMAS**

---

# 🤖 Laboratorio 01: Análisis, Diseño y Simulación de Circuitos con Arduino

* **Asignatura:** Robótica
* **Estudiante:** frankich99
* **Código / Correo Institucional:** [133962@unsaac.edu.pe](mailto:133962@unsaac.edu.pe)
* **Entorno de Simulación:** Autodesk Tinkercad
* **Plataforma Hardware:** Arduino Uno R3

---

## 📋 Tabla de Contenidos

1. [Objetivos del Laboratorio](#-objetivos-del-laboratorio)
2. [Estructura del Repositorio](#-estructura-del-repositorio)
3. [Circuito 1: Prender y Apagar un LED](#-circuito-1-prender-y-apagar-un-led)
4. [Circuito 2: Semáforo Americano](#-circuito-2-semáforo-americano)
5. [Circuito 3: Semáforo para Carros y Personas](#-circuito-3-semáforo-para-carros-y-personas)
6. [Circuito 4: Semáforo Peatonal por Demanda (Con Pulsador)](#-circuito-4-semáforo-peatonal-por-demanda-con-pulsador)
7. [Enlaces de Simulación en Tinkercad](#-enlaces-de-simulación-en-tinkercad)
8. [Enlace de la Conversación con IA](#-enlace-de-la-conversación-con-ia)

---

## 🎯 Objetivos del Laboratorio

* Analizar, diseñar y programar algoritmos de control en sistemas embebidos utilizando Arduino Uno R3.
* Comprender el funcionamiento de salidas digitales (control de actuadores luminosos y sonoros) y entradas digitales con pull-up interno.
* Implementar máquinas de estados para control de tráfico vehicular y peatonal.
* Aplicar temporización con retardos simples (`delay`) y temporización no bloqueante mediante funciones de tiempo de hardware (`millis`).
* Validar circuitos mediante simulación virtual interactiva en Autodesk Tinkercad.

---

## 📂 Estructura del Repositorio

| Archivo | Descripción | Plataforma |
| :--- | :--- | :--- |
| `1_prender_y_apagar_un_led1.ino` | Código fuente para parpadeo intermitente de un LED. | Tinkercad / Arduino IDE |
| `2_simular_el_funcionamiento_de_un_sem_foro_americano1.ino` | Código fuente del semáforo vehicular de tres tiempos (Verde, Amarillo, Rojo). | Tinkercad / Arduino IDE |
| `3_simular_de_sem_foro_para_carros_y_personas1.ino` | Código fuente del semáforo sincronizado para vehículos y peatones con alerta de parpadeo. | Tinkercad / Arduino IDE |
| `4_simular_el_funcionamiento_con_bot_n_peatonal_1.ino` | Código fuente del semáforo inteligente por demanda con pulsador, tiempo vehicular mínimo y señal acústica. | Tinkercad / Arduino IDE |
| `README.md` | Documentación técnica, diseño de circuitos y enlaces de entrega. | GitHub Markdown |

---

## 💡 Circuito 1: Prender y Apagar un LED

### 1.1. Análisis y Diseño
El circuito consiste en gobernar el encendido y apagado de un diodo emisor de luz (LED) a través de una salida digital del microcontrolador ATmega328P. Para limitar la corriente que circula por el diodo (evitando sobrecargas al pin de salida del Arduino), se conecta una resistencia limitadora de 220 Ω en serie con el ánodo del LED.

### 1.2. Mapeo de Conexiones
| Componente | Pin Arduino | Modo | Descripción |
| :--- | :--- | :--- | :--- |
| LED Rojo | Pin Digital 13 | `OUTPUT` | Señal de salida digital (5V / 0V) |
| Resistencia 220 Ω | Entre Pin 13 y Ánodo | - | Resistencia limitadora de corriente |
| Cátodo LED | GND | - | Retorno a masa |

### 1.3. Lógica del Algoritmo
* **Periodo:** 1000 ms encendido / 1000 ms apagado (Frecuencia: 0.5 Hz).
* **Archivo de código:** [`1_prender_y_apagar_un_led1.ino`](1_prender_y_apagar_un_led1.ino)
* **Simulación interactiva:** [Tinkercad - Circuito 1](https://www.tinkercad.com/things/dofh9KF4Y6d-1-prender-y-apagar-un-led)

---

## 🚦 Circuito 2: Semáforo Americano

### 1.1. Análisis y Diseño
Simula el ciclo tradicional de un semáforo vehicular simple con tres estados secuenciales mutuamente excluyentes:
1. **Verde (Paso libre):** 5000 ms.
2. **Amarillo (Transición y desaceleración preventiva):** 2000 ms.
3. **Rojo (Detención obligatoria):** 5000 ms.

### 1.2. Mapeo de Conexiones
| Componente | Pin Arduino | Modo | Función |
| :--- | :--- | :--- | :--- |
| LED Verde Vehicular | Pin Digital 7 | `OUTPUT` | Habilitación de flujo de vehículos |
| LED Amarillo Vehicular | Pin Digital 9 | `OUTPUT` | Advertencia de cambio de fase |
| LED Rojo Vehicular | Pin Digital 12 | `OUTPUT` | Detención vehicular obligatoria |
| Resistencias (x3) | 220 Ω cada una | - | Protección de diodos LED hacia GND |

### 1.3. Lógica del Algoritmo
* **Archivo de código:** [`2_simular_el_funcionamiento_de_un_sem_foro_americano1.ino`](2_simular_el_funcionamiento_de_un_sem_foro_americano1.ino)
* **Simulación interactiva:** [Tinkercad - Circuito 2](https://www.tinkercad.com/things/1r4FXvDILBR-2-simular-el-funcionamiento-de-un-semaforo-americano)

---

## 🚗🚶 Circuito 3: Semáforo para Carros y Personas

### 1.1. Análisis y Diseño
Regula el tránsito concurrente en una intersección de vehículos y peatones en un ciclo continuo sincronizado. Incluye:
* Estados complementarios: mientras los autos circulan (Verde), los peatones esperan (Rojo).
* **Fase de despeje / Seguridad (All-Red):** Un intervalo de 1 segundo en el que tanto vehículos como peatones se encuentran en rojo para garantizar que la calzada quede completamente vacía antes de autorizar el cruce opuesto.
* **Alerta de fin de cruce:** El verde peatonal destella antes de pasar a rojo, previniendo accidentes.

### 1.2. Mapeo de Conexiones
| Sección | Componente | Pin Arduino | Modo |
| :--- | :--- | :--- | :--- |
| **Vehicular** | LED Verde Autos | Pin Digital 2 | `OUTPUT` |
| | LED Amarillo Autos | Pin Digital 3 | `OUTPUT` |
| | LED Rojo Autos | Pin Digital 4 | `OUTPUT` |
| **Peatonal** | LED Verde Peatones | Pin Digital 5 | `OUTPUT` |
| | LED Rojo Peatones | Pin Digital 6 | `OUTPUT` |
| **Protección** | 5x Resistencias 220 Ω | Conectadas a GND | - |

### 1.3. Diagrama de Secuencia Temporal
1. **Fase 1:** Autos Verde (5s) + Peatones Rojo.
2. **Fase 2:** Autos Amarillo (2s) + Peatones Rojo.
3. **Fase 3:** Margen de seguridad (1s) - Rojo vehicular + Rojo peatonal.
4. **Fase 4:** Peatones Verde (5s) + Autos Rojo.
5. **Fase 5:** Destello Verde peatonal (4 pulsos de 300 ms) advirtiendo conclusión del cruce.
6. **Fase 6:** Margen de seguridad (1s) - Retorno seguro a fase vehicular.

* **Archivo de código:** [`3_simular_de_sem_foro_para_carros_y_personas1.ino`](3_simular_de_sem_foro_para_carros_y_personas1.ino)
* **Simulación interactiva:** [Tinkercad - Circuito 3](https://www.tinkercad.com/things/fTGLpI1pHEK-3-simular-de-semaforo-para-carros-y-personas)

---

## 🚸 Circuito 4: Semáforo Peatonal por Demanda (Con Pulsador)

### 1.1. Análisis y Diseño
Implementa un semáforo interactivo inteligente controlado por eventos:
* **Estado por defecto:** Flujo vehicular prioritario (Verde autos encendido permanentemente).
* **Atención de pulsador peatonal:** Se emplea una entrada digital con resistencia interna `INPUT_PULLUP` (Pin 6). Al presionarse, el pin cae a nivel bajo (`LOW`) y almacena la bandera de solicitud (`peticionCruce = true`).
* **Protección contra congestión:** Se garantiza un tiempo mínimo de tránsito vehicular (`T_VERDE_MIN = 6000 ms`) evaluado con `millis()`. La solicitud peatonal solo se ejecuta si ya transcurrieron al menos 6 segundos de flujo automotor continuo.
* **Señal Acústica / Aviso adicional:** Integra una señal sonora/actuador (Pin 3) activada durante la fase preventiva amarilla.

### 1.2. Mapeo de Conexiones
| Elemento | Dispositivo | Pin Arduino | Modo / Configuración |
| :--- | :--- | :--- | :--- |
| Tráfico Autos | LED Verde Vehicular | Pin Digital 11 | `OUTPUT` |
| | LED Amarillo Vehicular | Pin Digital 12 | `OUTPUT` |
| | LED Rojo Vehicular | Pin Digital 13 | `OUTPUT` |
| Tráfico Peatones | LED Verde Peatonal | Pin Digital 10 | `OUTPUT` |
| | LED Rojo Peatonal | Pin Digital 9 | `OUTPUT` |
| Control Demanda | Pulsador Peatonal (Push Button) | Pin Digital 6 | `INPUT_PULLUP` (Cierra a GND) |
| Actuador de Audio | Señal acústica / Buzzer | Pin Digital 3 | `OUTPUT` |
| Resistencias | 5x Resistencias 220 Ω / 330 Ω | Hacia GND | Limitación de corriente |

### 1.3. Lógica del Algoritmo
* Se evita el uso exclusivo de pausas bloqueantes durante la espera activa, asegurando que la lectura del pulsador sea detectada inmediatamente.
* **Archivo de código:** [`4_simular_el_funcionamiento_con_bot_n_peatonal_1.ino`](4_simular_el_funcionamiento_con_bot_n_peatonal_1.ino)
* **Simulación interactiva:** [Tinkercad - Circuito 4](https://www.tinkercad.com/things/4wRTOvmNFNS-4-simular-el-funcionamiento-con-boton-peatonal)

---

## 🔗 Enlaces de Simulación en Tinkercad

Acceso directo a las simulaciones interactivas públicas en Autodesk Tinkercad:

| N° | Ejercicio | Enlace a Simulación Tinkercad |
| :---: | :--- | :--- |
| **1** | Prender y apagar un LED | [🔗 Ver Simulación en Tinkercad](https://www.tinkercad.com/things/dofh9KF4Y6d-1-prender-y-apagar-un-led) |
| **2** | Simular el funcionamiento de un semáforo americano | [🔗 Ver Simulación en Tinkercad](https://www.tinkercad.com/things/1r4FXvDILBR-2-simular-el-funcionamiento-de-un-semaforo-americano) |
| **3** | Simular semáforo para carros y personas | [🔗 Ver Simulación en Tinkercad](https://www.tinkercad.com/things/fTGLpI1pHEK-3-simular-de-semaforo-para-carros-y-personas) |
| **4** | Simular funcionamiento con botón peatonal | [🔗 Ver Simulación en Tinkercad](https://www.tinkercad.com/things/4wRTOvmNFNS-4-simular-el-funcionamiento-con-boton-peatonal) |

---

## 💬 Enlace de la Conversación con IA

Como evidencia del proceso de interacción, formulación de prompts, análisis de requerimientos y desarrollo guiado del laboratorio, se adjunta el vínculo público de la conversación generada con el modelo de Inteligencia Artificial (Gemini):

* **Enlace de la conversación en Gemini:** [https://share.gemini.google/KPIcaHoQte1j](https://share.gemini.google/KPIcaHoQte1j)

---

> *Desarrollado para el curso de **Robótica** - Escuela Profesional de Ingeniería Informática y de Sistemas - **UNSAAC**.*
