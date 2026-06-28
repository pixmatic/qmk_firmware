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
// Enumeración de IDs de las teclas rápidas
enum speed_key_ids {
    SK_ID_ENT = 0,
    SK_ID_BSPC,
    SK_ID_DEL,
    SK_ID_UP,
    SK_ID_DOWN,
    SK_ID_LEFT,
    SK_ID_RGHT,
    SK_ID_PGUP,
    SK_ID_PGDN,
    SK_ID_PAST,        // ID para CTRL+V
    SPEED_KEYS_COUNT   // Cantidad total de teclas rápidas
};

enum speed_key_keycodes {
    RAPID_FIRE_START = MACRO_SAFE_RANGE,
    RAPID_FIRE_END = RAPID_FIRE_START + SPEED_KEYS_COUNT - 1,
    SPEED_KEY_SAFE_RANGE
};

// Macro para usar en el keymap (ej: SPEED_KEY(id))
#define SPEED_KEY(id) (RAPID_FIRE_START + (id))

// Alias de teclas rápidas comunes
#define SK_ENT  SPEED_KEY(SK_ID_ENT)
#define SK_BACK SPEED_KEY(SK_ID_BSPC)
#define SK_DEL  SPEED_KEY(SK_ID_DEL)
#define SK_UP   SPEED_KEY(SK_ID_UP)
#define SK_DOWN SPEED_KEY(SK_ID_DOWN)
#define SK_LEFT SPEED_KEY(SK_ID_LEFT)
#define SK_RIGH SPEED_KEY(SK_ID_RGHT)
#define SK_PGUP SPEED_KEY(SK_ID_PGUP)
#define SK_PGDW SPEED_KEY(SK_ID_PGDN)
#define SK_PAST SPEED_KEY(SK_ID_PAST)

/**
 * Intercepta y procesa las teclas de disparo rápido.
 * @param keycode El código de tecla pulsado.
 * @param record  El registro del evento de pulsación/liberación de QMK.
 * @return true si la tecla fue procesada por este módulo,
 *         false de lo contrario.
 */
bool process_speed_key(uint16_t keycode, keyrecord_t *record);
