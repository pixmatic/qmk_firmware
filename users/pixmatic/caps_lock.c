#include "quantum.h"
#include "pixmatic.h"

// Definición de las acciones de Tap Dance para Caps Lock en las teclas Shift
tap_dance_action_t tap_dance_actions[] = {
    [TD_LSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
    [TD_RSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_RSFT, KC_CAPS)
};
