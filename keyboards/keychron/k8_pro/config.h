#pragma once

/**
 * ==============================================================================
 * CONFIGURACIÓN DEL REGISTRO DE DESPLAZAMIENTO (MATRIX SCAN)
 * ==============================================================================
 * El Keychron K8 Pro utiliza un chip integrado '74HC595' (Shift Register) para
 * gestionar el escaneo de la matriz de teclas de forma serializada, ahorrando
 * pines físicos en el microcontrolador principal STM32L432.
 */

/* Pin STCP (Storage Clock / Latch): Controla el almacenamiento de los datos en el registro */
#define HC595_STCP A0

/* Pin SHCP (Shift Clock): Sincroniza el desplazamiento de los bits dentro del registro */
#define HC595_SHCP A1

/* Pin DS (Data Serial): Envía secuencialmente los bits de datos (0 o 1) al registro */
#define HC595_DS C15

/* Índice de inicio: Define la primera salida física del registro 74HC595 que se va a escanear */
#define HC595_START_INDEX 1

/* Índice de fin: Define la última salida física del registro que se utilizará para el escaneo */
#define HC595_END_INDEX 16

/**
 * ==============================================================================
 * CONFIGURACIÓN DE VELOCIDAD DEL BUS I2C (TIMING REGISTER)
 * ==============================================================================
 * Configuración de registros de tiempo para elevar la velocidad del bus I2C1 a
 * 1000 KHz (Fast-mode Plus). Esto evita el retardo (lag) en los efectos RGB.
 * Los valores configuran los ciclos de reloj internos del hardware de STMicroelectronics.
 */

/* I2C Prescaler: Divisor de frecuencia del reloj base para el periférico I2C (0U = sin división) */
#define I2C1_TIMINGR_PRESC 0U

/* SCL Delay: Tiempo de configuración del reloj de datos antes de enviar la señal */
#define I2C1_TIMINGR_SCLDEL 3U

/* SDA Delay: Tiempo de retardo de la señal de datos para garantizar lecturas estables */
#define I2C1_TIMINGR_SDADEL 0U

/* SCL High Period: Duración del estado lógico alto (1) en la línea de reloj I2C */
#define I2C1_TIMINGR_SCLH 15U

/* SCL Low Period: Duración del estado lógico bajo (0) en la línea de reloj I2C */
#define I2C1_TIMINGR_SCLL 51U

/**
 * ==============================================================================
 * CONFIGURACIÓN DEL DRIVER RGB (SNLED27351)
 * ==============================================================================
 * Este bloque de hardware de control lumínico solo se compilará si el driver de
 * Sonix está activo en la arquitectura interna del firmware.
 */

/* Pin Shutdown (SDB): Pin físico C14 encargado de encender (1) o apagar (0) el chip de luces */
#define SNLED27351_SDB_PIN C14

/* Dirección I2C del primer chip controlador: Mapeado a la línea de voltaje VDDIO de la placa */
#define SNLED27351_I2C_ADDRESS_1 SNLED27351_I2C_ADDRESS_VDDIO

/* Dirección I2C del segundo chip controlador: Mapeado a la línea de tierra (GND) de la placa */
#define SNLED27351_I2C_ADDRESS_2 SNLED27351_I2C_ADDRESS_GND

/* Ajuste de corriente por matriz: Configura la ganancia máxima de los canales de color (0x38) */
#define SNLED27351_CURRENT_TUNE \
    { 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38 }

/**
 * ==============================================================================
 * CONFIGURACIÓN DE LA MATRIZ RGB (ESTÁNDAR DE QMK)
 * ==============================================================================
 * Estas variables se exponen de forma fija para que el núcleo de QMK las lea
 * desde el primer milisegundo de la compilación.
 */

/* Cantidad de LEDs físicos: Total de luces RGB que QMK debe mapear y renderizar en el teclado */
#define RGB_MATRIX_LED_COUNT 87

/**
 * ==============================================================================
 * CONFIGURACIÓN DEL CONMUTADOR DE ALIMENTACIÓN Y INTERRUPTOR LATERAL
 * ==============================================================================
 * Permite leer el estado físico de los interruptores mecánicos externos del teclado
 * para poder utilizarlos como activadores lógicos en el mapa de teclas.
 */

/* Pin de selección BT/Cable: Pin A10 conectado al interruptor lateral (0 = Posición BT, 1 = Cable) */
#define BT_MODE_SELECT_PIN A10

/* Pin del sensor de energía USB: Pin B1 que monitoriza la presencia de voltaje en el puerto tipo C */
#define USB_POWER_SENSE_PIN B1

/* Nivel de conexión USB: Define que un estado bajo (0) en el pin B1 significa cable conectado */
#define USB_POWER_CONNECTED_LEVEL 0

/**
 * ==============================================================================
 * CONFIGURACIÓN DEL LED INDICADOR DE ESTADO / BATERÍA
 * ==============================================================================
 */

/* Pin del LED de carga: Pin A4 encargado de iluminar el indicador físico de batería/carga */
#define BAT_LOW_LED_PIN A4

/* Estado activo del LED: Define que el LED se enciende enviando un estado alto (1 = 3.3V) al pin */
#define BAT_LOW_LED_PIN_ON_STATE 1

/* Pin del LED indicador de estado de Bluetooth (Teclas 1, 2, 3) */
#define BT_INDICATION_LED_PIN H3
#define BT_INDICATION_LED_ON_STATE 1

/**
 * ==============================================================================
 * MAPEADO DE INTERCONEXIÓN DEL CHIP INALÁMBRICO BLUETOOTH
 * ==============================================================================
 * Pines de comunicación entre el procesador principal STM32L432 y el coprocesador
 * inalámbrico de Keychron. Se mantienen definidos para evitar errores de referencias cruzadas.
 */

/* Pin Reset de Bluetooth: Controla el pin de reinicio/apagado del chip CKBT51 */
#define CKBT51_RESET_PIN A9

/* Interrupción Wireless a MCU: Línea de aviso del chip Bluetooth hacia el STM32 (Datos entrantes) */
#define WIRELESS_TO_MCU_INT_PIN A6

/* Interrupción MCU a Wireless: Línea de aviso del STM32 hacia el chip Bluetooth (Datos salientes) */
#define MCU_TO_WIRELESS_INT_PIN A5
