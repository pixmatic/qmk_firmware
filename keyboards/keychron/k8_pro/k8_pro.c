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

void keyboard_pre_init_kb(void) {
    // 1. Apagamos físicamente el chip Bluetooth mandando un bajo lógico (0v) a su pin de Reset
    gpio_set_pin_output(CKBT51_RESET_PIN);
    gpio_write_pin_low(CKBT51_RESET_PIN);

    // 2. Configuramos el pin del interruptor lateral como entrada estable con resistencia Pull-Up
    gpio_set_pin_input_high(BT_MODE_SELECT_PIN);

    // 3. Inicializamos los LEDs de Bluetooth/Batería como salida y apagados por defecto
    gpio_set_pin_output(BAT_LOW_LED_PIN);
    gpio_write_pin(BAT_LOW_LED_PIN, !BAT_LOW_LED_PIN_ON_STATE);

    gpio_set_pin_output(BT_INDICATION_LED_PIN);
    gpio_write_pin(BT_INDICATION_LED_PIN, !BT_INDICATION_LED_ON_STATE);

    keyboard_pre_init_user();
}
