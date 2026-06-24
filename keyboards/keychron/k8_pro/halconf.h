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
 * CONFIGURACIÓN DE LA CAPA DE ABSTRACCIÓN DE HARDWARE (HAL) - CHIBIOS
 * ==============================================================================
 * Este archivo actúa como un panel de interruptores generales para el sistema
 * operativo en tiempo real (ChibiOS). Su función es activar o desactivar los
 * drivers de bajo nivel que controlan los periféricos físicos del microcontrolador.
 */

/* * Activación del protocolo I2C:
 * Si el teclado va a usar la matriz RGB, le ordenamos a ChibiOS que cargue en
 * memoria el driver del bus I2C. Esto abre los canales de comunicación hacia
 * el chip de luces y reserva recursos internos del procesador para su gestión.
 */
#define HAL_USE_I2C TRUE

/* * Directiva de herencia de configuración (#include_next):
 * Esta es una instrucción avanzada para el preprocesador de GCC. Le dice al
 * compilador: "Busca el siguiente archivo llamado 'halconf.h' en las carpetas
 * globales del sistema de QMK y aplícalo aquí".
 * * Gracias a esto, no tenemos que escribir las cientos de líneas que requiere ChibiOS
 * para el resto de periféricos (USB, almacenamiento, etc.); simplemente heredamos
 * la configuración estándar y estable de QMK y solo modificamos el I2C.
 */
#include_next <halconf.h>
