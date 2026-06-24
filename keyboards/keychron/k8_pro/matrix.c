/* Copyright 2026 pixmatic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"
#include <string.h>

/*
 * Definición de Pines del Registro de Desplazamiento (74HC595)
 *
 * La matriz utiliza un registro de desplazamiento 74HC595 para expandir los pines GPIO para la selección de columnas.
 * - HC595_STCP: Reloj del Registro de Almacenamiento (Pin Latch)
 * - HC595_SHCP: Reloj del Registro de Desplazamiento (Pin de Reloj/Clock)
 * - HC595_DS:   Entrada de Datos Serie (Pin de Datos)
 */
#ifndef HC595_STCP
#    error "HC595_STCP not defined"
#endif
#ifndef HC595_SHCP
#    error "HC595_SHCP not defined"
#endif
#ifndef HC595_DS
#    error "HC595_DS not defined"
#endif

/*
 * Rango y Desplazamiento de Columnas del Registro de Desplazamiento
 *
 * - HC595_START_INDEX: El índice de la primera columna de la matriz controlada por el registro de desplazamiento.
 * - HC595_END_INDEX:   El índice de la última columna de la matriz controlada por el registro de desplazamiento.
 * - HC595_OFFSET_INDEX: Desplazamiento para omitir pines iniciales específicos del registro de desplazamiento si es necesario.
 */
#ifndef HC595_START_INDEX
#    define HC595_START_INDEX 0
#endif
#ifndef HC595_END_INDEX
#    define HC595_END_INDEX 15
#endif
#ifndef HC595_OFFSET_INDEX
#    define HC595_OFFSET_INDEX 0
#endif

/*
 * Determina el tamaño de variable óptimo (entero de 8 bits, 16 bits o 32 bits)
 * para almacenar las máscaras de bits del registro de desplazamiento, según el número de columnas del mismo.
 * También define las máscaras de bits para deseleccionar y seleccionar todas las columnas.
 */
#if defined(HC595_START_INDEX) && defined(HC595_END_INDEX)
#    if ((HC595_END_INDEX - HC595_START_INDEX + 1) > 16)
#        define SIZE_T uint32_t
#        define UNSELECT_ALL_COL 0xFFFFFFFF
#        define SELECT_ALL_COL 0x00000000
#    elif ((HC595_END_INDEX - HC595_START_INDEX + 1) > 8)
#        define SIZE_T uint16_t
#        define UNSELECT_ALL_COL 0xFFFF
#        define SELECT_ALL_COL 0x0000
#    else
#        define SIZE_T uint8_t
#        define UNSELECT_ALL_COL 0xFF
#        define SELECT_ALL_COL 0x00
#    endif
#endif

// Mapeo de filas y columnas a los pines de hardware definidos en config.h
pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

/**
 * @brief Lee el estado de un pin de fila de la matriz.
 *
 * @param pin Representación pin_t del pin GPIO.
 * @return uint8_t 0 si la tecla está pulsada (conectada a GND a través de la columna seleccionada),
 *                 1 si la tecla no está pulsada (con pull-up a VCC) o si el pin no está definido.
 */
static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return gpio_read_pin(pin);
    } else {
        return 1; // Retorna 1 (no pulsado/alto) si no hay ningún pin físico mapeado
    }
}

/**
 * @brief Configura un pin GPIO como salida push-pull y lo establece en BAJO (LOW).
 * Útil para seleccionar columnas cableadas directamente.
 */
static inline void gpio_set_pin_output_push_pull_writeLow(pin_t pin) {
    gpio_set_pin_output_push_pull(pin);
    gpio_write_pin_low(pin);
}

/**
 * @brief Configura un pin GPIO como salida push-pull y lo establece en ALTO (HIGH).
 * Útil para deseleccionar columnas cableadas directamente cuando se requiere mantenerlas en alto.
 */
static inline void gpio_set_pin_output_push_pull_writeHigh(pin_t pin) {
    gpio_set_pin_output_push_pull(pin);
    gpio_write_pin_high(pin);
}

/**
 * @brief Función de retardo simple que utiliza NOPs de ensamblador para asegurar la estabilización
 * de la señal durante las transiciones de reloj del registro de desplazamiento.
 */
static inline void HC595_delay(uint16_t n) {
    while (n-- > 0) {
        asm volatile("nop" ::: "memory");
    }
}

/**
 * @brief Serializa los datos y los introduce en el registro de desplazamiento 74HC595, luego los activa (latch).
 *
 * Esta función se encarga de enviar una máscara de bits completa de todos los estados de columna
 * o un solo bit durante el progreso del escaneo. Se ejecuta dentro de un bloque ATOMIC_BLOCK para evitar
 * interrupciones en la temporización causadas por interrupciones del sistema.
 *
 * @param data La máscara de bits de estado de columna o el bit a desplazar.
 * @param bit_flag Si es true, solo desplaza el LSB (bit menos significativo) de data y sale
 *                 (usado para desplazar el cero móvil durante el escaneo).
 *                 Si es false, desplaza una máscara de bits completa correspondiente a todas las columnas del registro.
 */
static void HC595_output(SIZE_T data, bool bit_flag) {
    uint8_t n = 1; // Parámetro de retardo para la duración del pulso de reloj

    ATOMIC_BLOCK_FORCEON {
        // Itera a través de todos los pines conectados al registro de desplazamiento
        for (uint8_t i = 0; i < (HC595_END_INDEX - HC595_START_INDEX + 1); i++) {
            // Escribe el LSB actual en el pin DS (Datos Serie)
            if (data & 0x1) {
                gpio_write_pin_high(HC595_DS);
            } else {
                gpio_write_pin_low(HC595_DS);
            }
            // Genera un pulso en SHCP (Pin de Reloj del Registro de Desplazamiento) para introducir el bit
            gpio_write_pin_high(HC595_SHCP);
            HC595_delay(n);
            gpio_write_pin_low(HC595_SHCP);
            HC595_delay(n);

            if (bit_flag) {
                // Si está en modo de un solo bit, desplaza un bit y detiene el proceso de desplazamiento
                break;
            } else {
                // Desplaza los datos para registrar el siguiente bit
                data = data >> 1;
            }
        }
        // Genera un pulso en STCP (Reloj de Almacenamiento / Latch) para aplicar los valores desplazados a los pines de salida
        gpio_write_pin_high(HC595_STCP);
        HC595_delay(n);
        gpio_write_pin_low(HC595_STCP);
        HC595_delay(n);
    }
}

/**
 * @brief Selecciona (activa) una columna específica.
 *
 * La selección de columnas es activa en bajo (active-low):
 * - Las columnas GPIO directas se seleccionan escribiendo BAJO (LOW).
 * - Columnas del Registro de Desplazamiento:
 *   Utilizamos un método de cero móvil (walking-zero). Al comenzar en HC595_START_INDEX, introducimos un 0.
 *   Para las columnas siguientes, el 0 se desplaza automáticamente a su lugar durante unselect_col,
 *   por lo que no se requiere ninguna acción aquí para columnas mayores que HC595_START_INDEX.
 *
 * @param col Índice de la columna a seleccionar.
 */
static void select_col(uint8_t col) {
    if (col < HC595_START_INDEX || col > HC595_END_INDEX) {
        // Columna GPIO directa: configurar como salida y establecer en BAJO (seleccionar)
        gpio_set_pin_output_push_pull_writeLow(col_pins[col]);
    } else {
        // Columna del Registro de Desplazamiento:
        if (col == HC595_START_INDEX) {
            // Inicia la secuencia de cero móvil introduciendo un 0 en el registro de desplazamiento
            HC595_output(0x00, true);
            if (col < HC595_OFFSET_INDEX) {
                // Si hay una configuración de desplazamiento de pines (offset), introduce un 1 para desplazar el 0
                HC595_output(0x01, true);
            }
        }
    }
}

/**
 * @brief Deselecciona (desactiva) una columna específica.
 *
 * - Columnas GPIO directas: Se establecen en ALTO (HIGH) o en alta impedancia (input-pullup) según la configuración.
 * - Columnas del Registro de Desplazamiento: Introducimos un 1 (alto/deseleccionar) en el registro.
 *   Esto desplaza automáticamente el 0 existente (bajo/seleccionar) a la salida del registro de la siguiente columna,
 *   preparando la selección del cero móvil para la siguiente columna de la secuencia.
 *
 * @param col Índice de la columna a deseleccionar.
 */
static void unselect_col(uint8_t col) {
    if (col < HC595_START_INDEX || col > HC595_END_INDEX) {
        // Columna GPIO directa:
#ifdef MATRIX_UNSELECT_DRIVE_HIGH
        gpio_set_pin_output_push_pull_writeHigh(col_pins[col]);
#else
        gpio_set_pin_input_high(col_pins[col]);
#endif
    } else {
        // Columna del Registro de Desplazamiento: introduce un 1 para desactivar este pin y desplazar el 0 hacia adelante
        HC595_output(0x01, true);
    }
}

/**
 * @brief Deselecciona todas las columnas de la matriz para evitar el efecto fantasma (ghosting) o ruido inicial.
 */
static void unselect_cols(void) {
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        if (col < HC595_START_INDEX || col > HC595_END_INDEX) {
            // Columna GPIO directa
#ifdef MATRIX_UNSELECT_DRIVE_HIGH
            gpio_set_pin_output_push_pull_writeHigh(col_pins[col]);
#else
            gpio_set_pin_input_high(col_pins[col]);
#endif
        } else {
            // Columnas del Registro de Desplazamiento: las limpia todas a la vez al llegar al índice de inicio
            if (col == HC595_START_INDEX) {
                HC595_output(UNSELECT_ALL_COL, false); // Desplaza unos (1s) a todas las salidas del registro de desplazamiento
            }
            break; // No es necesario procesar individualmente las columnas restantes del registro de desplazamiento
        }
    }
}

/**
 * @brief Selecciona todas las columnas de la matriz.
 * Se utiliza para la detección de eventos en toda la matriz o para el encendido al pulsar una tecla.
 */
void select_all_cols(void) {
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        if (col < HC595_START_INDEX || col > HC595_END_INDEX) {
            // Columna GPIO directa: escribir bajo para seleccionar
            gpio_set_pin_output_push_pull_writeLow(col_pins[col]);
        } else {
            // Columnas del Registro de Desplazamiento: las selecciona todas a la vez al llegar al índice de inicio
            if (col == HC595_START_INDEX) {
                HC595_output(SELECT_ALL_COL, false); // Desplaza ceros (0s) a todas las salidas del registro de desplazamiento
            }
            break; // No es necesario procesar individualmente las columnas restantes del registro de desplazamiento
        }
    }
}

/**
 * @brief Selecciona una columna, lee el estado de todas las filas en esa columna,
 *        y actualiza el arreglo de estado de la matriz.
 *
 * @param current_matrix Puntero al arreglo que contiene los estados de la matriz sin procesar.
 * @param current_col El índice de la columna a escanear.
 * @param row_shifter Máscara de bits que representa el bit de la columna (ej. 1 << current_col).
 */
static void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {
    // Selecciona la columna
    select_col(current_col);
    HC595_delay(200); // Espera a que las señales se estabilicen antes de leer

    // Para cada fila, lee el estado del pin
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Si el pin de la fila lee 0 (BAJO/LOW), significa que la tecla está pulsada (cortocircuitada a tierra)
        if (readMatrixPin(row_pins[row_index]) == 0) {
            // El pin está en BAJO, establece el bit correspondiente a current_col
            current_matrix[row_index] |= row_shifter;
        } else {
            // El pin está en ALTO, limpia el bit correspondiente a current_col
            current_matrix[row_index] &= ~row_shifter;
        }
    }

    // Deselecciona la columna
    unselect_col(current_col);
    HC595_delay(200); // Espera a que todas las señales de las filas vuelvan a ALTO a través de las resistencias de pull-up
}

/**
 * @brief Inicializa el hardware de la matriz de teclado personalizada.
 *
 * Configura los pines de control del registro de desplazamiento como salida (bajo) e inicializa los pines de fila como entrada con pull-up.
 */
void matrix_init_custom(void) {
    // Configura los pines de control del registro de desplazamiento como salida push-pull
    gpio_set_pin_output_push_pull(HC595_DS);
    gpio_write_pin_low(HC595_DS);
    gpio_set_pin_output_push_pull(HC595_STCP);
    gpio_write_pin_low(HC595_STCP);
    gpio_set_pin_output_push_pull(HC595_SHCP);
    gpio_write_pin_low(HC595_SHCP);

    // Configura todos los pines de fila activos como entrada con resistencias de pull-up habilitadas
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        if (row_pins[x] != NO_PIN) {
            gpio_set_pin_input_high(row_pins[x]);
        }
    }

    // Establece las columnas en su estado por defecto deseleccionado
    unselect_cols();
}

/**
 * @brief Escanea la matriz de teclado personalizada y actualiza el estado de la misma.
 *
 * QMK core llama a esta función de forma repetida. Lee cada columna secuencialmente,
 * comprueba los estados de las filas y los compara con el estado de la matriz anterior.
 *
 * @param current_matrix Puntero al arreglo que contiene el estado actual del teclado.
 * @return true si el estado de la matriz ha cambiado desde el último escaneo.
 * @return false si el estado de la matriz no ha cambiado.
 */
bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    matrix_row_t curr_matrix[MATRIX_ROWS] = {0};

    // Establece la columna y lee las filas para cada columna
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        matrix_read_rows_on_col(curr_matrix, current_col, row_shifter);
    }

    // Comprueba si el estado de la matriz ha cambiado desde el último escaneo
    bool changed = memcmp(current_matrix, curr_matrix, sizeof(curr_matrix)) != 0;
    if (changed) {
        // Copia el nuevo estado a current_matrix
        memcpy(current_matrix, curr_matrix, sizeof(curr_matrix));
    }

    return changed;
}
