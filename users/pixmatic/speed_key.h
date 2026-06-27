#pragma once
#include "quantum.h"
#include "macros.h"

// ==============================================================================
// CONFIGURACIÓN DE VELOCIDAD DE DISPARO (AMETRALLADORA)
// ==============================================================================
// Intervalo en milisegundos entre cada pulsación.
// Menor número = Mayor velocidad. (50 ms = 20 pulsaciones por segundo).
#define SPEED_KEY_DELAY 5

// ==============================================================================
// GESTIÓN DE RANGOS DE KEYCODES
// ==============================================================================
enum speed_key_keycodes {
    RAPID_FIRE_START = MACRO_SAFE_RANGE,
    RAPID_FIRE_END = RAPID_FIRE_START + 0xFF, // Reservamos 256 keycodes básicos
    SPEED_KEY_SAFE_RANGE
};

// Macro para usar en el keymap (ej: SPEED_KEY(kc))
#define SPEED_KEY(kc) (RAPID_FIRE_START + (kc))

// Alias de teclas rápidas comunes
#define SK_ENT  SPEED_KEY(KC_ENT)
#define SK_BACK SPEED_KEY(KC_BSPC)
#define SK_DEL  SPEED_KEY(KC_DEL)
#define SK_UP   SPEED_KEY(KC_UP)
#define SK_DOWN SPEED_KEY(KC_DOWN)
#define SK_LEFT SPEED_KEY(KC_LEFT)
#define SK_RIGH SPEED_KEY(KC_RGHT)
#define SK_PGUP SPEED_KEY(KC_PGUP)
#define SK_PGDW SPEED_KEY(KC_PGDN)

/**
 * Intercepta y procesa las teclas de disparo rápido.
 * @param keycode El código de tecla pulsado.
 * @param record  El registro del evento de pulsación/liberación de QMK.
 * @return true si la tecla fue procesada por este módulo,
 *         false de lo contrario.
 */
bool process_speed_key(uint16_t keycode, keyrecord_t *record);
