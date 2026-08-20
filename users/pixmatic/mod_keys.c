#include "quantum.h"
#include "pixmatic.h"

// Variables de estado para el Tap Dance C_24 (Caps Lock personalizado)
// `c24_layer_is_held`: Rastrea si la tecla de Hold está físicamente presionada y activa en la Capa 2.
static bool c24_layer_is_held = false;
// `c24_layer_3_was_active`, `c24_layer_4_was_active`, `c24_layer_5_was_active` y `c24_layer_6_was_active`:
// Almacenan si las capas superiores estaban activas antes de iniciar el Hold.
static bool c24_layer_3_was_active = false;
static bool c24_layer_4_was_active = false;
static bool c24_layer_5_was_active = false;
static bool c24_layer_6_was_active = false;

// Variables de estado para el Tap Dance C_13 (Fn1 personalizado)
// `c13_layer_is_held`: Rastrea si la tecla de Hold está físicamente presionada y activa en la Capa 1.
static bool c13_layer_is_held = false;
// `c13_layer_2_was_active` a `c13_layer_6_was_active`: Almacenan el estado previo de capas superiores.
static bool c13_layer_2_was_active = false;
static bool c13_layer_3_was_active = false;
static bool c13_layer_4_was_active = false;
static bool c13_layer_5_was_active = false;
static bool c13_layer_6_was_active = false;

// Variables de estado para el Tap Dance C_56 (Fn2 personalizado)
// `c56_layer_is_held`: Rastrea si la tecla de Hold está físicamente presionada y activa en la Capa 5.
static bool c56_layer_is_held = false;
static bool c56_layer_6_was_active = false;

/**
 * @brief Acción de finalización del Tap Dance C_24 (Hold -> Capa 2, Tap -> Capa 4)
 * 
 * Se ejecuta cuando el temporizador de Tap Dance determina el resultado de la pulsación:
 * - Si es Hold (mantenido): Guarda el estado de las capas superiores (3, 4, 5, 6), las apaga si estaban encendidas
 *   para dejar libre el paso en la pila de capas de QMK, y activa momentáneamente la Capa 2.
 * - Si es Tap (toque rápido): Alterna el estado de activación de la Capa 4 de manera exclusiva
 *   con las capas 3 y 6 (si alguna estaba activa, se apaga primero).
 */
void dance_c24_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // --- Operación de Hold (Ir a Capa 2) ---
        c24_layer_3_was_active = layer_state_is(3);
        c24_layer_4_was_active = layer_state_is(4);
        c24_layer_5_was_active = layer_state_is(5);
        c24_layer_6_was_active = layer_state_is(6);
        
        if (c24_layer_3_was_active) {
            layer_off(3);
        }
        if (c24_layer_4_was_active) {
            layer_off(4);
        }
        if (c24_layer_5_was_active) {
            layer_off(5);
        }
        if (c24_layer_6_was_active) {
            layer_off(6);
        }
        
        layer_on(2);
        c24_layer_is_held = true;
    } else {
        // --- Operación de Tap (Alternar Capa 4) ---
        if (layer_state_is(4)) {
            layer_off(4);
        } else {
            layer_off(3);
            layer_off(6);
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
 * - Restaura y re-activa las capas que estaban activas antes de iniciar el Hold.
 */
void dance_c24_reset(tap_dance_state_t *state, void *user_data) {
    if (c24_layer_is_held) {
        layer_off(2);
        
        if (c24_layer_3_was_active) {
            layer_on(3);
            c24_layer_3_was_active = false;
        }
        if (c24_layer_4_was_active) {
            layer_on(4);
            c24_layer_4_was_active = false;
        }
        if (c24_layer_5_was_active) {
            layer_on(5);
            c24_layer_5_was_active = false;
        }
        if (c24_layer_6_was_active) {
            layer_on(6);
            c24_layer_6_was_active = false;
        }
        c24_layer_is_held = false;
    }
}

/**
 * @brief Acción de finalización del Tap Dance C_13 (Hold -> Capa 1, Tap -> Capa 3)
 * 
 * Sigue el mismo principio de resolución de precedencia de capas que C_24:
 * - Si es Hold (mantenido): Guarda el estado de las capas 2, 3, 4, 5 y 6, las desactiva si estuvieran encendidas,
 *   y activa momentáneamente la Capa 1 (Multimedia / Macros).
 * - Si es Tap (toque rápido): Alterna la activación de la Capa 3 de forma exclusiva con las capas 4 y 6.
 */
void dance_c13_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // --- Operación de Hold (Ir a Capa 1) ---
        c13_layer_2_was_active = layer_state_is(2);
        c13_layer_3_was_active = layer_state_is(3);
        c13_layer_4_was_active = layer_state_is(4);
        c13_layer_5_was_active = layer_state_is(5);
        c13_layer_6_was_active = layer_state_is(6);
        
        if (c13_layer_2_was_active) {
            layer_off(2);
        }
        if (c13_layer_3_was_active) {
            layer_off(3);
        }
        if (c13_layer_4_was_active) {
            layer_off(4);
        }
        if (c13_layer_5_was_active) {
            layer_off(5);
        }
        if (c13_layer_6_was_active) {
            layer_off(6);
        }
        
        layer_on(1);
        c13_layer_is_held = true;
    } else {
        // --- Operación de Tap (Alternar Capa 3) ---
        if (layer_state_is(3)) {
            layer_off(3);
        } else {
            layer_off(4);
            layer_off(6);
            layer_on(3);
        }
    }
}

/**
 * @brief Acción de restauración/reset del Tap Dance C_13
 * 
 * Se ejecuta al liberar la tecla física C_13 tras un Hold.
 * - Desactiva la Capa 1.
 * - Re-activa cada capa superior a su estado original previo al Hold.
 */
void dance_c13_reset(tap_dance_state_t *state, void *user_data) {
    if (c13_layer_is_held) {
        layer_off(1);
        
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
        if (c13_layer_5_was_active) {
            layer_on(5);
            c13_layer_5_was_active = false;
        }
        if (c13_layer_6_was_active) {
            layer_on(6);
            c13_layer_6_was_active = false;
        }
        c13_layer_is_held = false;
    }
}

/**
 * @brief Acción de finalización del Tap Dance C_56 (Hold -> Capa 5, Tap -> Capa 6)
 * 
 * - Si es Hold (mantenido): Guarda el estado de la Capa 6, la desactiva si estuviera encendida,
 *   y activa momentáneamente la Capa 5 (Teclas F1-F12).
 * - Si es Tap (toque rápido): Alterna la activación de la Capa 6 de forma exclusiva con las capas 3 y 4.
 */
void dance_c56_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // --- Operación de Hold (Ir a Capa 5) ---
        c56_layer_6_was_active = layer_state_is(6);
        
        if (c56_layer_6_was_active) {
            layer_off(6);
        }
        
        layer_on(5);
        c56_layer_is_held = true;
    } else {
        // --- Operación de Tap (Alternar Capa 6) ---
        if (layer_state_is(6)) {
            layer_off(6);
        } else {
            layer_off(3);
            layer_off(4);
            layer_on(6);
        }
    }
}

/**
 * @brief Acción de restauración/reset del Tap Dance C_56
 * 
 * Se ejecuta al liberar la tecla física C_56 tras un Hold.
 * - Desactiva la Capa 5.
 * - Re-activa la Capa 6 si se encontraba encendida con anterioridad.
 */
void dance_c56_reset(tap_dance_state_t *state, void *user_data) {
    if (c56_layer_is_held) {
        layer_off(5);
        
        if (c56_layer_6_was_active) {
            layer_on(6);
            c56_layer_6_was_active = false;
        }
        c56_layer_is_held = false;
    }
}

// Configuración y registro oficial de las acciones de Tap Dance en QMK.
// Mapea los identificadores de pixmatic.h a las funciones correspondientes.
tap_dance_action_t tap_dance_actions[] = {
    [TD_C24_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_c24_finished, dance_c24_reset),
    [TD_C13_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_c13_finished, dance_c13_reset),
    [TD_C56_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_c56_finished, dance_c56_reset)
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

