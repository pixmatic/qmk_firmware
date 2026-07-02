#include "quantum.h"
#include "pixmatic.h"

// Estado para controlar si el Tap Dance de C_24 está retenido (hold)
static bool c24_layer_is_held = false;

// Estado para controlar si el Tap Dance de C_13 está retenido (hold)
static bool c13_layer_is_held = false;

// Acción al terminar el baile de C_24 (Hold -> Capa 2, Tap -> Capa 4)
void dance_c24_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // Si se mantiene presionado (Hold): activar la capa 2 momentáneamente
        layer_on(2);
        c24_layer_is_held = true;
    } else {
        // Si se pulsa y suelta rápido (Tap): alternar la capa 4 de forma exclusiva con la capa 3
        if (layer_state_is(4)) {
            layer_off(4);
        } else {
            layer_off(3);
            layer_on(4);
        }
    }
}

// Acción al restablecer el baile de C_24
void dance_c24_reset(tap_dance_state_t *state, void *user_data) {
    if (c24_layer_is_held) {
        layer_off(2);
        c24_layer_is_held = false;
    }
}

// Acción al terminar el baile de C_13 (Hold -> Capa 1, Tap -> Capa 3)
void dance_c13_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        // Si se mantiene presionado (Hold): activar la capa 1 (Macros) momentáneamente
        layer_on(1);
        c13_layer_is_held = true;
    } else {
        // Si se pulsa y suelta rápido (Tap): alternar la capa 3 de forma exclusiva con la capa 4
        if (layer_state_is(3)) {
            layer_off(3);
        } else {
            layer_off(4);
            layer_on(3);
        }
    }
}

// Acción al restablecer el baile de C_13
void dance_c13_reset(tap_dance_state_t *state, void *user_data) {
    if (c13_layer_is_held) {
        layer_off(1);
        c13_layer_is_held = false;
    }
}

// Definición de las acciones de Tap Dance del espacio de usuario
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

