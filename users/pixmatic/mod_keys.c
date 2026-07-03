#include "quantum.h"
#include "pixmatic.h"

// Variables de estado para el Tap Dance C_24 (Caps Lock personalizado)
// `c24_layer_is_held`: Rastrea si la tecla de Hold está físicamente presionada y activa en la Capa 2.
static bool c24_layer_is_held = false;
// `c24_layer_3_was_active` y `c24_layer_4_was_active`: Almacenan si las capas superiores (3 y 4)
// estaban activas antes de iniciar el Hold. Esto nos permite desactivarlas temporalmente para evitar que
// ensombrezcan (shadow) a la Capa 2 (que es de menor índice), y restaurarlas exactamente al liberar la tecla.
static bool c24_layer_3_was_active = false;
static bool c24_layer_4_was_active = false;

// Variables de estado para el Tap Dance C_13 (Fn personalizado)
// `c13_layer_is_held`: Rastrea si la tecla de Hold está físicamente presionada y activa en la Capa 1.
static bool c13_layer_is_held = false;
// `c13_layer_2_was_active`, `c13_layer_3_was_active` y `c13_layer_4_was_active`: Almacenan el estado
// previo de las capas superiores a la Capa 1. Dado que QMK procesa la capa activa más alta con prioridad,
// cualquier capa superior activa ensombrecería la Capa 1 si no las desactiváramos de forma temporal.
static bool c13_layer_2_was_active = false;
static bool c13_layer_3_was_active = false;
static bool c13_layer_4_was_active = false;

/**
 * @brief Acción de finalización del Tap Dance C_24 (Hold -> Capa 2, Tap -> Capa 4)
 * 
 * Se ejecuta cuando el temporizador de Tap Dance determina el resultado de la pulsación:
 * - Si es Hold (mantenido): Guarda el estado de las capas 3 y 4, las apaga si estaban encendidas
 *   para dejar libre el paso en la pila de capas de QMK, y activa momentáneamente la Capa 2.
 * - Si es Tap (toque rápido): Alterna el estado de activación de la Capa 4 de manera exclusiva
 *   con la Capa 3 (si la capa 3 estaba activa, se apaga primero).
 */
void dance_c24_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // --- Operación de Hold (Ir a Capa 2) ---
        // 1. Guardamos el estado actual de las capas superiores que podrían interferir.
        c24_layer_3_was_active = layer_state_is(3);
        c24_layer_4_was_active = layer_state_is(4);
        
        // 2. Desactivamos temporalmente las capas superiores. Esto solventa el bug de ensombrecimiento
        //    donde la Capa 4 activa (que es superior a la 2) impedía usar los keycodes de la Capa 2.
        if (c24_layer_3_was_active) {
            layer_off(3);
        }
        if (c24_layer_4_was_active) {
            layer_off(4);
        }
        
        // 3. Activamos la capa destino (Capa 2) y registramos el estado de hold.
        layer_on(2);
        c24_layer_is_held = true;
    } else {
        // --- Operación de Tap (Alternar Capa 4) ---
        // Alternamos de forma exclusiva: si la capa 4 está activa, la apagamos;
        // de lo contrario, nos aseguramos de apagar la 3 y encender la 4.
        if (layer_state_is(4)) {
            layer_off(4);
        } else {
            layer_off(3);
            layer_on(4);
        }
    }
}

/**
 * @brief Acción de restauración/reset del Tap Dance C_24
 * 
 * Se ejecuta en cuanto la tecla física C_24 se libera (evento de release del Hold).
 * Revierte los cambios de capas del Hold para dejar el teclado en el estado previo al Hold:
 * - Apaga la Capa 2.
 * - Restaura y re-activa la Capa 3 y/o Capa 4 si se determinó que estaban activas antes de iniciar el Hold.
 */
void dance_c24_reset(tap_dance_state_t *state, void *user_data) {
    if (c24_layer_is_held) {
        // 1. Apagamos la capa que activamos momentáneamente.
        layer_off(2);
        
        // 2. Restauramos el estado de las capas que apagamos temporalmente.
        if (c24_layer_3_was_active) {
            layer_on(3);
            c24_layer_3_was_active = false;
        }
        if (c24_layer_4_was_active) {
            layer_on(4);
            c24_layer_4_was_active = false;
        }
        c24_layer_is_held = false;
    }
}

/**
 * @brief Acción de finalización del Tap Dance C_13 (Hold -> Capa 1, Tap -> Capa 3)
 * 
 * Sigue el mismo principio de resolución de precedencia de capas que C_24:
 * - Si es Hold (mantenido): Guarda el estado de las capas 2, 3 y 4, las desactiva si estuvieran encendidas,
 *   y activa momentáneamente la Capa 1 (Macros).
 * - Si es Tap (toque rápido): Alterna la activación de la Capa 3 de forma exclusiva con la Capa 4.
 */
void dance_c13_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // --- Operación de Hold (Ir a Capa 1) ---
        // 1. Guardamos el estado de todas las capas superiores a la capa destino (Capa 1).
        c13_layer_2_was_active = layer_state_is(2);
        c13_layer_3_was_active = layer_state_is(3);
        c13_layer_4_was_active = layer_state_is(4);
        
        // 2. Desactivamos temporalmente estas capas superiores para evitar que ensombrezcan la Capa 1.
        if (c13_layer_2_was_active) {
            layer_off(2);
        }
        if (c13_layer_3_was_active) {
            layer_off(3);
        }
        if (c13_layer_4_was_active) {
            layer_off(4);
        }
        
        // 3. Activamos la capa destino (Capa 1) y registramos el estado de hold.
        layer_on(1);
        c13_layer_is_held = true;
    } else {
        // --- Operación de Tap (Alternar Capa 3) ---
        // Alternamos de forma exclusiva con la capa 4.
        if (layer_state_is(3)) {
            layer_off(3);
        } else {
            layer_off(4);
            layer_on(3);
        }
    }
}

/**
 * @brief Acción de restauración/reset del Tap Dance C_13
 * 
 * Se ejecuta al liberar la tecla física C_13 tras un Hold.
 * - Desactiva la Capa 1.
 * - Vuelve a encender las capas 2, 3 y 4 si se encontraban encendidas con anterioridad.
 */
void dance_c13_reset(tap_dance_state_t *state, void *user_data) {
    if (c13_layer_is_held) {
        // 1. Desactivamos la capa temporal del Hold (Capa 1).
        layer_off(1);
        
        // 2. Re-activamos cada capa superior a su estado original previo al Hold.
        if (c13_layer_2_was_active) {
            layer_on(2);
            c13_layer_2_was_active = false;
        }
        if (c13_layer_3_was_active) {
            layer_on(3);
            c13_layer_3_was_active = false;
        }
        if (c13_layer_4_was_active) {
            layer_on(4);
            c13_layer_4_was_active = false;
        }
        c13_layer_is_held = false;
    }
}

// Configuración y registro oficial de las acciones de Tap Dance en QMK.
// Mapea los identificadores de pixmatic.h a las funciones correspondientes.
tap_dance_action_t tap_dance_actions[] = {
    [TD_C24_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_c24_finished, dance_c24_reset),
    [TD_C13_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_c13_finished, dance_c13_reset)
};

// Estado físico y lógico de los Shift para control de Caps Lock por doble Shift pulsado
static bool lsft_pressed = false;
static bool rsft_pressed = false;
static bool lsft_registered = false;
static bool rsft_registered = false;

bool process_shift_caps(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_LSFT) {
        if (record->event.pressed) {
            lsft_pressed = true;
            if (rsft_pressed) {
                if (rsft_registered) {
                    unregister_code(KC_RSFT);
                    rsft_registered = false;
                }
                tap_code(KC_CAPS);
                lsft_registered = false;
                return false;
            } else {
                lsft_registered = true;
                return true;
            }
        } else {
            lsft_pressed = false;
            if (lsft_registered) {
                lsft_registered = false;
                return true;
            } else {
                return false;
            }
        }
    } else if (keycode == KC_RSFT) {
        if (record->event.pressed) {
            rsft_pressed = true;
            if (lsft_pressed) {
                if (lsft_registered) {
                    unregister_code(KC_LSFT);
                    lsft_registered = false;
                }
                tap_code(KC_CAPS);
                rsft_registered = false;
                return false;
            } else {
                rsft_registered = true;
                return true;
            }
        } else {
            rsft_pressed = false;
            if (rsft_registered) {
                rsft_registered = false;
                return true;
            } else {
                return false;
            }
        }
    }

    return true;
}

