#pragma once
#include "quantum.h"

// Mínimo común para Tap Dance
enum {
    TD_LSFT_CAPS = 0,
    TD_RSFT_CAPS,
    TD_C24_LAYER,
    TD_C13_LAYER
};

// Redefinimos _______ a KC_NO
#undef _______
#define _______ KC_NO

// Alias para la tecla de Tap Dance Shift
#define TD_SHIF TD(TD_LSFT_CAPS)

// Alias para la tecla C_24 (Hold -> Layer 2, Tap -> Toggle Layer 4)
#define C_24 TD(TD_C24_LAYER)

// Alias para la tecla C_13 (Hold -> Layer 1, Tap -> Toggle Layer 3)
#define C_13 TD(TD_C13_LAYER)

#if !defined(PIXMATIC_C)
#    include "mod_keys.c"
#endif

#include "macros.h"
#include "speed_key.h"
