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

#pragma once

/**
 * ==============================================================================
 * CONFIGURACIÓN ESPECÍFICA DEL MICROCONTROLADOR (MCU) - CHIBIOS
 * ==============================================================================
 * Este archivo configura las entrañas del chip STM32L432. Regula la velocidad
 * del procesador, los multiplicadores de frecuencia del reloj principal (Clock)
 * y la asignación de hardware dedicada para cada periférico.
 */

/* * Directiva de herencia de configuración (#include_next):
 * Carga en primer lugar el archivo 'mcuconf.h' oficial y genérico que QMK tiene
 * asignado para la arquitectura STM32L432. Esto nos da una base de funcionamiento
 * segura sobre la que aplicar nuestros cambios quirúrgicos abajo.
 */
#include_next <mcuconf.h>

/**
 * ==============================================================================
 * CONFIGURACIÓN DE LOS MULTIPLICADORES DE RELOJ (PLL - PHASE-LOCKED LOOP)
 * ==============================================================================
 * Modifica la forma en que el chip multiplica la frecuencia de su cristal de cuarzo
 * externo para generar la velocidad del procesador. Es vital para mantener síncrono
 * el puerto USB del teclado y evitar retardos o desconexiones.
 */

/* * Divisor de entrada PLL (M):
 * Anula la configuración por defecto y establece el divisor en 2. Modifica la
 * frecuencia de entrada que va hacia el multiplicador principal.
 */
#undef STM32_PLLM_VALUE
#define STM32_PLLM_VALUE 2

/* * Multiplicador principal PLL (N):
 * Anula la configuración por defecto y establece el factor de multiplicación en 12.
 * En combinación con el valor M anterior, ajusta el reloj interno a la frecuencia
 * exacta requerida por el diseño de la placa base del Keychron K8 Pro.
 */
#undef STM32_PLLN_VALUE
#define STM32_PLLN_VALUE 12

/**
 * ==============================================================================
 * ASIGNACIÓN DE HARDWARE PARA EL CANAL DE ILUMINACIÓN
 * ==============================================================================
 */

/* * Activación del canal físico I2C1:
 * El microcontrolador tiene varios motores internos para gestionar conexiones I2C.
 * Aquí anulamos la directiva por defecto del sistema operativo y forzamos el encendido
 * específico del canal 'I2C1' de la placa, que es la tubería de hardware real donde
 * están soldados físicamente los chips controladores de tus luces.
 */
#undef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1 TRUE
