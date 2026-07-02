#pragma once
#include "quantum.h"

#include "macros.h"
#include "speed_key.h"

// Mínimo común para Tap Dance
enum {
    TD_C24_LAYER = 0,
    TD_C13_LAYER
};

// Redefinimos _______ a KC_NO
#undef _______
#define _______ KC_NO

// Rango de keycodes personalizados de pixmatic
enum pixmatic_keycodes {
    PIXMATIC_SAFE_RANGE = SPEED_KEY_SAFE_RANGE
};

// Alias para la tecla C_24 (Hold -> Layer 2, Tap -> Toggle Layer 4)
#define C_24 TD(TD_C24_LAYER)

// Alias para la tecla C_13 (Hold -> Layer 1, Tap -> Toggle Layer 3)
#define C_13 TD(TD_C13_LAYER)

// Funciones del gestor de modificadores
bool process_shift_caps(uint16_t keycode, keyrecord_t *record);

#if !defined(PIXMATIC_C)
#    include "mod_keys.c"
#endif

