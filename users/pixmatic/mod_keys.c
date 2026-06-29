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

#define SH_CAPS_MIN_TAP_DELAY 80  // Mínimo de milisegundos entre pulsaciones para considerarlo un doble toque intencionado
#define SH_CAPS_MAX_TAP_DELAY 500 // Máximo de milisegundos para considerarlo un toque rápido (tap)

// Variables de estado para el comportamiento personalizado de Shift/Caps Lock sin retardo
static uint8_t shift_press_count = 0;
static bool shift_interrupted = false;
static uint32_t shift_press_time = 0;
static uint32_t last_shift_release_time = 0;
static bool shift_is_double_tap = false;

bool process_shift_caps(uint16_t keycode, keyrecord_t *record) {
    if (keycode == SH_CAPS) {
        if (record->event.pressed) {
            shift_press_count++;
            if (shift_press_count == 1) {
                shift_interrupted = false;
                shift_press_time = timer_read32();
                uint32_t elapsed = timer_elapsed32(last_shift_release_time);
                if (last_shift_release_time != 0 && elapsed < SH_CAPS_MAX_TAP_DELAY && elapsed >= SH_CAPS_MIN_TAP_DELAY) {
                    tap_code(KC_CAPS);
                    shift_is_double_tap = true;
                    last_shift_release_time = 0;
                } else {
                    shift_is_double_tap = false;
                    register_code(KC_LSFT);
                }
            }
        } else {
            if (shift_press_count > 0) {
                shift_press_count--;
                if (shift_press_count == 0) {
                    if (shift_is_double_tap) {
                        shift_is_double_tap = false;
                    } else {
                        unregister_code(KC_LSFT);
                        if (!shift_interrupted && timer_elapsed32(shift_press_time) < SH_CAPS_MAX_TAP_DELAY) {
                            last_shift_release_time = timer_read32();
                        } else {
                            last_shift_release_time = 0;
                        }
                    }
                }
            }
        }
        return false; // Interceptado, no procesar más en QMK
    }

    // Si se presiona cualquier otra tecla mientras Shift está activo, se interrumpe el tap de Shift
    if (record->event.pressed && shift_press_count > 0) {
        shift_interrupted = true;
    }

    return true; // Continuar procesamiento normal
}

